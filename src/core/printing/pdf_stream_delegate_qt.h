// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef PDF_STREAM_DELEGATE_QT_H
#define PDF_STREAM_DELEGATE_QT_H

#include "components/pdf/browser/pdf_stream_delegate.h"

// based on chrome/browser/pdf/chrome_pdf_stream_delegate.h:
class PdfStreamDelegateQt : public pdf::PdfStreamDelegate
{
public:
    PdfStreamDelegateQt();
    PdfStreamDelegateQt(const PdfStreamDelegateQt &) = delete;
    PdfStreamDelegateQt operator=(const PdfStreamDelegateQt &) = delete;
    ~PdfStreamDelegateQt() override;

    // pdf::PdfStreamDelegate:
    std::optional<GURL> MapToOriginalUrl(content::NavigationHandle &navigation_handle) override;
    std::optional<StreamInfo> GetStreamInfo(content::RenderFrameHost *embedder_frame) override;
    void OnPdfEmbedderSandboxed(content::FrameTreeNodeId) override;
    bool ShouldAllowPdfFrameNavigation(content::NavigationHandle*) override;
};

#endif // PDF_STREAM_DELEGATE_QT_H
