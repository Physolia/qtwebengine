// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.Chromium file.

#include "clipboard_qt.h"
#include "clipboard_change_observer.h"
#include "type_conversion.h"

#include "base/containers/map_util.h"
#include "base/logging.h"
#include "base/strings/utf_offset_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/types/variant_util.h"
#include "base/types/optional_util.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/custom_data_helper.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/clipboard/clipboard_monitor.h"
#include "ui/base/clipboard/clipboard_constants.h"
#include "ui/base/clipboard/clipboard_format_type.h"
#include "ui/base/data_transfer_policy/data_transfer_endpoint.h"
#include "ui/base/ui_base_features.h"

#include <QBuffer>
#include <QGuiApplication>
#include <QImage>
#include <QImageWriter>
#include <QMimeData>

#include <memory>

using namespace Qt::StringLiterals;

namespace QtWebEngineCore {

static void registerMetaTypes()
{
    qRegisterMetaType<QClipboard::Mode>("QClipboard::Mode");
}

Q_CONSTRUCTOR_FUNCTION(registerMetaTypes)

Q_GLOBAL_STATIC(ClipboardChangeObserver, clipboardChangeObserver)

ClipboardChangeObserver::ClipboardChangeObserver()
{
    connect(QGuiApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), SLOT(trackChange(QClipboard::Mode)));
}

void ClipboardChangeObserver::trackChange(QClipboard::Mode mode)
{
    if (mode == QClipboard::Clipboard)
        m_primarySequenceNumber = ui::ClipboardSequenceNumberToken();
    else if (mode == QClipboard::Selection)
        m_selectionSequenceNumber = ui::ClipboardSequenceNumberToken();
    else
        return;
    ui::ClipboardMonitor::GetInstance()->NotifyClipboardDataChanged();
}

} // namespace QtWebEngineCore

using namespace QtWebEngineCore;

namespace {

std::unique_ptr<QMimeData> uncommittedData;
QMimeData *getUncommittedData()
{
    if (!uncommittedData)
        uncommittedData.reset(new QMimeData);
    return uncommittedData.get();
}

} // namespace

namespace ui {

// Factory function
Clipboard *Clipboard::Create()
{
    return new ClipboardQt;
}

} // namespace ui

namespace QtWebEngineCore {

#if defined(Q_OS_WIN)
extern std::string HtmlToCFHtml(const std::string &html, const std::string &base_url);
extern void CFHtmlExtractMetadata(const std::string &cf_html, std::string *base_url,
                                  size_t *html_start, size_t *fragment_start, size_t *fragment_end);
#endif // defined(Q_OS_WIN)

void ClipboardQt::WritePortableAndPlatformRepresentations(ui::ClipboardBuffer type,
                                                          const ObjectMap &objects,
                                                          std::vector<ui::Clipboard::PlatformRepresentation> platform_representations,
                                                          std::unique_ptr<ui::DataTransferEndpoint> data_src, uint32_t val)
{
    DCHECK(CalledOnValidThread());
    DCHECK(IsSupportedClipboardBuffer(type));

    if (!platform_representations.empty())
        DispatchPlatformRepresentations(std::move(platform_representations));
    for (const auto &object : objects)
        DispatchPortableRepresentation(object.second);

    // Commit the accumulated data.
    if (uncommittedData)
        QGuiApplication::clipboard()->setMimeData(uncommittedData.release(),
                                                  type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard
                                                                                          : QClipboard::Selection);

    if (type == ui::ClipboardBuffer::kCopyPaste && IsSupportedClipboardBuffer(ui::ClipboardBuffer::kSelection)) {
        auto text_iter = objects.find(base::VariantIndexOfType<Data, TextData>());
        if (text_iter != objects.end()) {
            // Copy text and SourceTag to the selection clipboard.
            WritePortableAndPlatformRepresentations(ui::ClipboardBuffer::kSelection,
                                                    ObjectMap(text_iter, ++text_iter),
                                                    {},
                                                    nullptr, val);
        }
    }
    m_dataSrc[type] = std::move(data_src);
}

void ClipboardQt::WriteText(std::string_view text)
{
    getUncommittedData()->setText(toQString(text));
}

void ClipboardQt::WriteHTML(std::string_view markup, std::optional<std::string_view> source_url)
{

    QString markup_string;
#if defined (Q_OS_MACOS)
    // We need to prepend the charset on macOS to prevent garbled Unicode characters
    // when pasting to certain applications (e.g. Notes, TextEdit)
    // Mirrors the behavior in ui/base/clipboard/clipboard_mac.mm in Chromium.
    markup_string += "<meta charset='utf-8'>"_L1;
#endif
    markup_string += toQString(markup);

#if !defined(Q_OS_WIN)
    getUncommittedData()->setHtml(markup_string);
#else
    QString url;
    if (source_url)
        url = toQString(*source_url);

    std::string cf_html = HtmlToCFHtml(markup_string.toStdString(), url.toStdString());
    size_t html_start = std::string::npos;
    size_t fragment_start = std::string::npos;
    size_t fragment_end = std::string::npos;
    CFHtmlExtractMetadata(cf_html, nullptr, &html_start, &fragment_start, &fragment_end);

    DCHECK(fragment_start != std::string::npos && fragment_end != std::string::npos
            && html_start != std::string::npos);
    DCHECK(fragment_start >= html_start && fragment_end >= fragment_start);

    getUncommittedData()->setHtml(QString::fromStdString(cf_html.substr(html_start)));
#endif // !defined(Q_OS_WIN)
}

void ClipboardQt::WriteRTF(std::string_view rtf)
{
    getUncommittedData()->setData(QString::fromLatin1(ui::kMimeTypeRTF), toQByteArray(rtf));
}

void ClipboardQt::WriteWebSmartPaste()
{
    getUncommittedData()->setData(QString::fromLatin1(ui::kMimeTypeWebkitSmartPaste), QByteArray());
}

void ClipboardQt::WriteBitmap(const SkBitmap &bitmap)
{
    getUncommittedData()->setImageData(toQImage(bitmap).copy());
}

void ClipboardQt::WriteBookmark(std::string_view title_in, std::string_view url_in)
{
    // FIXME: Untested, seems to be used only for drag-n-drop.
    // Write as a mozilla url (UTF16: URL, newline, title).
    QString url = toQString(url_in);
    QString title = toQString(title_in);

    QByteArray data;
    data.append(reinterpret_cast<const char *>(url.utf16()), url.size() * 2);
    data.append('\n');
    data.append(reinterpret_cast<const char *>(title.utf16()), title.size() * 2);
    getUncommittedData()->setData(QString::fromLatin1(ui::kMimeTypeMozillaURL), data);
}

void ClipboardQt::WriteData(const ui::ClipboardFormatType &format, base::span<const uint8_t> data)
{
    getUncommittedData()->setData(QString::fromStdString(format.GetName()), QByteArray((const char *)data.data(), data.size()));
}

bool ClipboardQt::IsFormatAvailable(const ui::ClipboardFormatType &format,
                                    ui::ClipboardBuffer type,
                                    const ui::DataTransferEndpoint *data_dst) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);

    if (!mimeData)
        return false;
    if (format == ui::ClipboardFormatType::PngType())
        return mimeData->hasImage();
    return mimeData->hasFormat(QString::fromStdString(format.GetName()));
}

void ClipboardQt::Clear(ui::ClipboardBuffer type)
{
    QGuiApplication::clipboard()->clear(type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard
                                                                                : QClipboard::Selection);
    m_dataSrc[type].reset();
}

void ClipboardQt::ReadAvailableTypes(ui::ClipboardBuffer type,
                                     const ui::DataTransferEndpoint *data_dst,
                                     std::vector<std::u16string> *types) const
{
    if (!types) {
        NOTREACHED();
        return;
    }

    types->clear();
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;

    for (const auto& mime_type : GetStandardFormats(type, data_dst))
        types->push_back(mime_type);

    const QString serializedDataTransferCustomType =
            QString::fromStdString(ui::ClipboardFormatType::DataTransferCustomType().Serialize());
    if (mimeData->hasFormat(serializedDataTransferCustomType)) {
        const QByteArray customData = mimeData->data(serializedDataTransferCustomType);
        const base::span custom_data(customData.constData(), (unsigned long)customData.size());
        ui::ReadCustomDataTypes(base::as_bytes(custom_data), types);
    }
}

void ClipboardQt::ReadText(ui::ClipboardBuffer type,
                           const ui::DataTransferEndpoint *data_dst,
                           std::u16string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (mimeData)
        *result = toString16(mimeData->text());
}

void ClipboardQt::ReadAsciiText(ui::ClipboardBuffer type,
                                const ui::DataTransferEndpoint *data_dst,
                                std::string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (mimeData)
        *result = mimeData->text().toStdString();
}

void ClipboardQt::ReadHTML(ui::ClipboardBuffer type,
                           const ui::DataTransferEndpoint *data_dst,
                           std::u16string *markup, std::string *src_url,
                           uint32_t *fragment_start, uint32_t *fragment_end) const
{
    markup->clear();
    if (src_url)
        src_url->clear();
    *fragment_start = 0;
    *fragment_end = 0;

    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;

#if !defined(Q_OS_WIN)
    *markup = toString16(mimeData->html());
    *fragment_end = static_cast<uint32_t>(markup->length());
#else
    const std::string cf_html = mimeData->html().toStdString();
    size_t html_start = std::string::npos;
    size_t start_index = std::string::npos;
    size_t end_index = std::string::npos;
    CFHtmlExtractMetadata(cf_html, src_url, &html_start, &start_index, &end_index);

    // This might happen if the contents of the clipboard changed and CF_HTML is
    // no longer available.
    if (start_index == std::string::npos || end_index == std::string::npos
        || html_start == std::string::npos)
        return;

    if (start_index < html_start || end_index < start_index)
        return;

    std::vector<size_t> offsets;
    offsets.push_back(start_index - html_start);
    offsets.push_back(end_index - html_start);
    markup->assign(base::UTF8ToUTF16AndAdjustOffsets(cf_html.data() + html_start, &offsets));
    // Ensure the Fragment points within the string; see https://crbug.com/607181.
    size_t end = std::min(offsets[1], markup->length());
    *fragment_start = base::checked_cast<uint32_t>(std::min(offsets[0], end));
    *fragment_end = base::checked_cast<uint32_t>(end);
#endif // !defined(Q_OS_WIN)
}

void ClipboardQt::ReadRTF(ui::ClipboardBuffer type,
                          const ui::DataTransferEndpoint *data_dst,
                          std::string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;
    const QByteArray byteArray = mimeData->data(QString::fromLatin1(ui::kMimeTypeRTF));
    *result = std::string(byteArray.constData(), byteArray.length());
}

void ClipboardQt::ReadPng(ui::ClipboardBuffer type, const ui::DataTransferEndpoint *, ui::Clipboard::ReadPngCallback callback) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return std::move(callback).Run({});
    QImage image = qvariant_cast<QImage>(mimeData->imageData());

    QBuffer buffer;
    QImageWriter writer(&buffer, "png");
    writer.write(image);
    std::vector<uint8_t> pngData;
    pngData.resize(buffer.size());
    memcpy(pngData.data(), buffer.data().data(), buffer.size());
    return std::move(callback).Run(std::move(pngData));
}

void ClipboardQt::ReadDataTransferCustomData(ui::ClipboardBuffer clipboard_type, const std::u16string &type,
                                             const ui::DataTransferEndpoint *data_dst,
                                             std::u16string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            clipboard_type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;
    const QString serializedDataTransferCustomType =
            QString::fromStdString(ui::ClipboardFormatType::DataTransferCustomType().Serialize());
    const QByteArray customData = mimeData->data(serializedDataTransferCustomType);
    const base::span custom_data(customData.constData(), (unsigned long)customData.size());
    if (auto maybe_result = ui::ReadCustomDataForType(base::as_bytes(custom_data), type))
        *result = *std::move(maybe_result);
}

void ClipboardQt::ReadBookmark(const ui::DataTransferEndpoint *data_dst, std::u16string *title, std::string *url) const
{
    NOTIMPLEMENTED();
}

void ClipboardQt::ReadSvg(ui::ClipboardBuffer clipboard_type,
                          const ui::DataTransferEndpoint *,
                          std::u16string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            clipboard_type == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;
    const QByteArray svgData = mimeData->data(QString::fromLatin1(ui::kMimeTypeSvg));
    if (!svgData.isEmpty())
        *result = toString16(QString::fromUtf8(svgData));
}

void ClipboardQt::WriteSvg(std::string_view markup)
{
    getUncommittedData()->setData(QString::fromLatin1(ui::kMimeTypeSvg),
                                  toQByteArray(markup));
}

void ClipboardQt::ReadData(const ui::ClipboardFormatType &format,
                           const ui::DataTransferEndpoint *data_dst,
                           std::string *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData();
    if (!mimeData)
        return;
    const QByteArray byteArray = mimeData->data(QString::fromStdString(format.GetName()));
    *result = std::string(byteArray.constData(), byteArray.length());
}

const ui::ClipboardSequenceNumberToken &ClipboardQt::GetSequenceNumber(ui::ClipboardBuffer type) const
{
    return type == ui::ClipboardBuffer::kCopyPaste
            ? clipboardChangeObserver()->getPrimarySequenceNumber()
            : clipboardChangeObserver()->getSelectionSequenceNumber();
}

std::optional<ui::DataTransferEndpoint> ClipboardQt::GetSource(ui::ClipboardBuffer buffer) const
{
    return base::OptionalFromPtr(base::FindPtrOrNull(m_dataSrc, buffer));
}

void ClipboardQt::ReadFilenames(ui::ClipboardBuffer buffer,
                                const ui::DataTransferEndpoint *data_dst,
                                std::vector<ui::FileInfo> *result) const
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            buffer == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return;
    const QList<QUrl> urls = mimeData->urls();
    for (const QUrl &url : urls) {
        if (url.isLocalFile()) {
            base::FilePath filepath = toFilePath(url.toLocalFile());
            result->push_back(ui::FileInfo(filepath, base::FilePath()));
        }
    }
}

void ClipboardQt::WriteFilenames(std::vector<ui::FileInfo> filenames)
{
    QList<QUrl> urls;
    for (const ui::FileInfo &file : filenames) {
        QUrl url = QUrl::fromLocalFile(QString::fromStdString(file.path.AsUTF8Unsafe()));
        urls.append(url);
    }
    getUncommittedData()->setUrls(urls);
}

#if BUILDFLAG(IS_OZONE)
bool ClipboardQt::IsSelectionBufferAvailable() const
{
    return QGuiApplication::clipboard()->supportsSelection();
}
#endif

void ClipboardQt::WriteClipboardHistory()
{
    NOTIMPLEMENTED();
}
void ClipboardQt::WriteUploadCloudClipboard()
{
    NOTIMPLEMENTED();
}
void ClipboardQt::WriteConfidentialDataForPassword()
{
    NOTIMPLEMENTED();
}

// This is the same as ReadAvailableTypes minus dealing with custom-data
std::vector<std::u16string> ClipboardQt::GetStandardFormats(ui::ClipboardBuffer buffer, const ui::DataTransferEndpoint *data_dst) const
{
    Q_UNUSED(data_dst);
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData(
            buffer == ui::ClipboardBuffer::kCopyPaste ? QClipboard::Clipboard : QClipboard::Selection);
    if (!mimeData)
        return {};

    std::vector<std::u16string> types;
    if (mimeData->hasImage())
        types.push_back(base::UTF8ToUTF16(ui::kMimeTypePNG));
    if (mimeData->hasHtml())
        types.push_back(base::UTF8ToUTF16(ui::kMimeTypeHTML));
    if (mimeData->hasText())
        types.push_back(base::UTF8ToUTF16(ui::kMimeTypeText));
    if (mimeData->hasUrls())
        types.push_back(base::UTF8ToUTF16(ui::kMimeTypeURIList));
    const QStringList formats = mimeData->formats();
    for (const QString &mimeType : formats) {
        auto mime_type = mimeType.toStdString();
        // Only add white-listed formats here
        if (mime_type == ui::ClipboardFormatType::SvgType().GetName() ||
            mime_type == ui::ClipboardFormatType::RtfType().GetName()) {
          types.push_back(base::UTF8ToUTF16(mime_type));
          continue;
        }
    }
    return types;
}

} // namespace QtWebEngineCore
