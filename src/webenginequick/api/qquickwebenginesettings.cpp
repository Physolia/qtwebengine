// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickwebengineprofile.h"
#include "qquickwebenginesettings_p.h"

#include "web_engine_settings.h"

QT_BEGIN_NAMESPACE

QQuickWebEngineSettings::QQuickWebEngineSettings(QQuickWebEngineSettings *parentSettings)
    : d_ptr(new QWebEngineSettings(parentSettings ? parentSettings->d_ptr.data() : nullptr))
{ }

/*!
    \qmltype WebEngineSettings
    //! \nativetype QQuickWebEngineSettings
    \inqmlmodule QtWebEngine
    \since QtWebEngine 1.1
    \brief Allows configuration of browser properties and attributes.

    The WebEngineSettings type can be used to configure browser properties and generic
    attributes, such as JavaScript support, focus behavior, and access to remote content. This type
    is uncreatable, but the default settings for all web engine views can be accessed by using
    the \l [QML] {WebEngine::settings}{WebEngine.settings} property.

    Each web engine view can have individual settings that can be accessed by using the
    \l{WebEngineView::settings}{WebEngineView.settings} property.
*/


QQuickWebEngineSettings::~QQuickWebEngineSettings()
{ }

/*!
    \qmlproperty bool WebEngineSettings::autoLoadImages

    Automatically loads images on web pages.

    Enabled by default.
*/
bool QQuickWebEngineSettings::autoLoadImages() const
{
    return d_ptr->testAttribute(QWebEngineSettings::AutoLoadImages);
}

/*!
    \qmlproperty bool WebEngineSettings::javascriptEnabled

    Enables the running of JavaScript programs.

    Enabled by default.
*/
bool QQuickWebEngineSettings::javascriptEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::JavascriptEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::javascriptCanOpenWindows

    Allows JavaScript programs to open popup windows without user interaction.

    Enabled by default.
*/
bool QQuickWebEngineSettings::javascriptCanOpenWindows() const
{
    return d_ptr->testAttribute(QWebEngineSettings::JavascriptCanOpenWindows);
}

/*!
    \qmlproperty bool WebEngineSettings::javascriptCanAccessClipboard

    Allows JavaScript programs to write (copy) sanitized content to the clipboard. A
    sanitized write is done with the \c{write} and \c{writeText} JavaScript Clipboard API
    calls and must be accompanied by user action.

    Unsanitized writes, and reading from the clipboard, are
    enabled by \l{javascriptCanPaste}.

    Prior to Chromium version 81, this setting enabled all clipboard writes.

    Since unrestricted clipboard access is a potential security concern, it is
    recommended that applications leave this disabled and instead respond to
    \l{webEnginePermission::permissionType}{ClipboardReadWrite} feature permission requests.

    Disabled by default.
*/
bool QQuickWebEngineSettings::javascriptCanAccessClipboard() const
{
    return d_ptr->testAttribute(QWebEngineSettings::JavascriptCanAccessClipboard);
}

/*!
    \qmlproperty bool WebEngineSettings::linksIncludedInFocusChain

    Includes hyperlinks in the keyboard focus chain.

    Enabled by default.
*/
bool QQuickWebEngineSettings::linksIncludedInFocusChain() const
{
    return d_ptr->testAttribute(QWebEngineSettings::LinksIncludedInFocusChain);
}

/*!
    \qmlproperty bool WebEngineSettings::localStorageEnabled

    Enables support for the HTML 5 local storage feature.

    Enabled by default.
*/
bool QQuickWebEngineSettings::localStorageEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::LocalStorageEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::localContentCanAccessRemoteUrls

    Allows locally loaded documents to access remote URLs.

    Disabled by default.
*/
bool QQuickWebEngineSettings::localContentCanAccessRemoteUrls() const
{
    return d_ptr->testAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls);
}

/*!
    \qmlproperty bool WebEngineSettings::spatialNavigationEnabled

    Enables the Spatial Navigation feature, which means the ability to navigate between focusable
    elements, such as hyperlinks and form controls, on a web page by using the Left, Right, Up and
    Down arrow keys.

    For example, if a user presses the Right key, heuristics determine whether there is an element
    they might be trying to reach towards the right and which element they probably want.

    Disabled by default.

*/
bool QQuickWebEngineSettings::spatialNavigationEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::SpatialNavigationEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::localContentCanAccessFileUrls

    Allows locally loaded documents to access other local URLs.

    Enabled by default.
*/
bool QQuickWebEngineSettings::localContentCanAccessFileUrls() const
{
    return d_ptr->testAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls);
}

/*!
    \qmlproperty bool WebEngineSettings::hyperlinkAuditingEnabled

    Enables support for the \c ping attribute for hyperlinks.

    Disabled by default.
*/
bool QQuickWebEngineSettings::hyperlinkAuditingEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::HyperlinkAuditingEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::errorPageEnabled

    Enables displaying the built-in error pages of Chromium.

    Enabled by default.
*/
bool QQuickWebEngineSettings::errorPageEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ErrorPageEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::pluginsEnabled

    Enables support for Pepper plugins, such as the Flash player.

    Disabled by default.
*/
bool QQuickWebEngineSettings::pluginsEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PluginsEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::fullscreenSupportEnabled
    \since QtWebEngine 1.2

    Tells the web engine whether fullscreen is supported in this application or not.

    Disabled by default.
*/
bool QQuickWebEngineSettings::fullScreenSupportEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::FullScreenSupportEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::screenCaptureEnabled
    \since QtWebEngine 1.3

    Tells the web engine whether screen capture is supported in this application or not.

    Disabled by default.
*/
bool QQuickWebEngineSettings::screenCaptureEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ScreenCaptureEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::webGLEnabled
    \since QtWebEngine 1.3

    Enables support for HTML 5 WebGL.

    Enabled by default if available.
*/
bool QQuickWebEngineSettings::webGLEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::WebGLEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::accelerated2dCanvasEnabled
    \since QtWebEngine 1.3

    Specifies whether the HTML 5 2D canvas should be an OpenGL framebuffer.
    This makes many painting operations faster, but slows down pixel access.

    Enabled by default if available.
*/
bool QQuickWebEngineSettings::accelerated2dCanvasEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled);
}

/*!
  \qmlproperty bool WebEngineSettings::autoLoadIconsForPage
  \since QtWebEngine 1.3

  Automatically downloads icons for web pages.

  Enabled by default.
*/
bool QQuickWebEngineSettings::autoLoadIconsForPage() const
{
    return d_ptr->testAttribute(QWebEngineSettings::AutoLoadIconsForPage);
}

/*!
  \qmlproperty bool WebEngineSettings::touchIconsEnabled
  \since QtWebEngine 1.3

  Enables support for touch icons and precomposed touch icons.

  Disabled by default.
*/
bool QQuickWebEngineSettings::touchIconsEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::TouchIconsEnabled);
}

/*!
  \qmlproperty bool WebEngineSettings::focusOnNavigationEnabled
  \since QtWebEngine 1.4

  Focus is given to the view whenever a navigation operation occurs
  (load, stop, reload, reload and bypass cache, forward, backward, set content, and so on).

  Disabled by default.
*/
bool QQuickWebEngineSettings::focusOnNavigationEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::FocusOnNavigationEnabled);
}

/*!
  \qmlproperty bool WebEngineSettings::printElementBackgrounds
  \since QtWebEngine 1.4

  Turns on printing of CSS backgrounds when printing a web page.

  Enabled by default.
*/
bool QQuickWebEngineSettings::printElementBackgrounds() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PrintElementBackgrounds);
}

/*!
  \qmlproperty bool WebEngineSettings::allowRunningInsecureContent
  \since QtWebEngine 1.4

  By default, HTTPS pages cannot run JavaScript, CSS, plugins or
  web-sockets from HTTP URLs. This used to be possible and this
  provides an override to get the old behavior.

  Disabled by default.
*/
bool QQuickWebEngineSettings::allowRunningInsecureContent() const
{
    return d_ptr->testAttribute(QWebEngineSettings::AllowRunningInsecureContent);
}

/*!
  \qmlproperty bool WebEngineSettings::allowGeolocationOnInsecureOrigins
  \since QtWebEngine 1.5

  Since Qt 5.7, only secure origins such as HTTPS have been able to request
  Geolocation features. This provides an override to allow non secure
  origins to access Geolocation again.

  Disabled by default.
*/
bool QQuickWebEngineSettings::allowGeolocationOnInsecureOrigins() const
{
    return d_ptr->testAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins);
}

/*!
  \qmlproperty bool WebEngineSettings::allowWindowActivationFromJavaScript
  \since QtWebEngine 1.6
  Allows the window.focus() method in JavaScript. Disallowed by default.
*/
bool QQuickWebEngineSettings::allowWindowActivationFromJavaScript() const
{
    return d_ptr->testAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript);
}

/*!
  \qmlproperty bool WebEngineSettings::showScrollBars
  \since QtWebEngine 1.6
  Shows scroll bars. Enabled by default.
*/
bool QQuickWebEngineSettings::showScrollBars() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ShowScrollBars);
}

/*!
  \qmlproperty bool WebEngineSettings::playbackRequiresUserGesture
  \since QtWebEngine 1.7
  Inhibits playback of media content until the user interacts with
  the page.

  By default, Qt WebEngine uses Chromium settings, as described in
  \l {Autoplay Policy Changes}. To overwrite the default behavior,
  this property must be set to \c false.

  \note The behavior is similar to Chrome on Android when enabled,
  and similar to Chrome on desktops when disabled.
*/
bool QQuickWebEngineSettings::playbackRequiresUserGesture() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PlaybackRequiresUserGesture);
}

/*!
  \qmlproperty bool WebEngineSettings::webRTCPublicInterfacesOnly
  \since QtWebEngine 1.7
  Limits WebRTC to public IP addresses only. When disabled WebRTC may also use
  local network IP addresses, but remote hosts can also see your local network
  IP address.

  Disabled by default.
*/
bool QQuickWebEngineSettings::webRTCPublicInterfacesOnly() const
{
    return d_ptr->testAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly);
}

/*!
    \qmlproperty bool WebEngineSettings::javascriptCanPaste
    \since QtWebEngine 1.7

    Allows JavaScript programs to read (paste) from the clipboard and to write unsanitized
    content. A sanitized write is done with the \c{write} and \c{writeText} JavaScript
    Clipboard API calls and must be accompanied by user action; unsanitized writes are any
    writes which do not meet these criteria.

    For this setting to have any effect, \l{javascriptCanAccessClipboard} must also be
    enabled.

    Since unrestricted clipboard access is a potential security concern, it is
    recommended that applications leave this disabled and instead respond to
    \l{webEnginePermission::permissionType}{ClipboardReadWrite} feature permission requests.

    Disabled by default.
*/
bool QQuickWebEngineSettings::javascriptCanPaste() const
{
    return d_ptr->testAttribute(QWebEngineSettings::JavascriptCanPaste);
}

/*!
    \qmlproperty bool WebEngineSettings::dnsPrefetchEnabled
    \since QtWebEngine 1.8

    Enables speculative prefetching of DNS records for HTML links before
    they are activated.

    Disabled by default.
*/
bool QQuickWebEngineSettings::dnsPrefetchEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::DnsPrefetchEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::pdfViewerEnabled
    \since QtWebEngine 1.9

    Specifies that PDF documents will be opened in the internal PDF viewer
    instead of being downloaded.

    Enabled by default.
*/
bool QQuickWebEngineSettings::pdfViewerEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PdfViewerEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::navigateOnDropEnabled
    \since QtWebEngine 6.4

    Specifies that navigations can be triggered by dropping URLs on
    the view.

    Enabled by default.
*/
bool QQuickWebEngineSettings::navigateOnDropEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::NavigateOnDropEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::readingFromCanvasEnabled
    \since QtWebEngine 6.6

    Specifies that reading from all canvas elements is enabled.

    This setting will have impact on all HTML5 canvas elements irrespective of origin, and can be disabled
    to prevent canvas fingerprinting.

    Enabled by default.
 */
bool QQuickWebEngineSettings::readingFromCanvasEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ReadingFromCanvasEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::forceDarkMode
    \since QtWebEngine 6.7

    Automatically render all web contents using a dark theme.

    Disabled by default.
 */
bool QQuickWebEngineSettings::forceDarkMode() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ForceDarkMode);
}

/*!
    \qmlproperty bool WebEngineSettings::printHeaderAndFooter
    \since QtWebEngine 6.9

    Specifies that printing results will contain basic page information (URL,
    title, date, page number) in header and footer area. It is the embedder's
    responsibility to provide enough space for these texts by setting proper top
    and bottom margins.

    Disabled by default.
 */
bool QQuickWebEngineSettings::printHeaderAndFooter() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PrintHeaderAndFooter);
}

/*!
    \qmlproperty bool WebEngineSettings::preferCSSMarginsForPrinting
    \since QtWebEngine 6.9

    Turns on preferring CSS margins over the default (0, 0, 0, 0) sizes when
    printing a web page.

    Disabled by default.
*/
bool QQuickWebEngineSettings::preferCSSMarginsForPrinting() const
{
    return d_ptr->testAttribute(QWebEngineSettings::PreferCSSMarginsForPrinting);
}

/*!
    \qmlproperty bool WebEngineSettings::scrollAnimatorEnabled
    \since QtWebEngine 6.8

    Enables animated scrolling.

    Disabled by default.
 */
bool QQuickWebEngineSettings::scrollAnimatorEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::ScrollAnimatorEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::touchEventsApiEnabled
    \since QtWebEngine 6.9

    Enables support for JavaScript touch events API, meaning \c ontouchstart,
    \c ontouchend and \c ontouchmove handlers will be present in the \c document.window object.

    Note that some websites use this API to decide whether they run on a mobile device or on desktop
    and base their design on it. This can cause unwanted results on touchscreen laptops or other
    setups that emulate a fake touch device.

    Enabled by default if a touch device detected by the system
    and disabled otherwise.
 */
bool QQuickWebEngineSettings::touchEventsApiEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::TouchEventsApiEnabled);
}

/*!
    \qmlproperty bool WebEngineSettings::backForwardCacheEnabled
    \since QtWebEngine 6.10

    Enables support for back/forward cache (or bfcache) to speed up back and
    forward navigation.

    Disabled by default.
*/
bool QQuickWebEngineSettings::backForwardCacheEnabled() const
{
    return d_ptr->testAttribute(QWebEngineSettings::BackForwardCacheEnabled);
}

/*!
    \qmlproperty string WebEngineSettings::defaultTextEncoding
    \since QtWebEngine 1.2

    Sets the default encoding. The value must be a string describing an encoding such as "utf-8" or
    "iso-8859-1".

    If left empty, a default value will be used.
*/
QString QQuickWebEngineSettings::defaultTextEncoding() const
{
    return d_ptr->defaultTextEncoding();
}

ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::ImageAnimationPolicy::Allow,
                   QWebEngineSettings::ImageAnimationPolicy::Allow)
ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::ImageAnimationPolicy::AnimateOnce,
                   QWebEngineSettings::ImageAnimationPolicy::AnimateOnce)
ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::ImageAnimationPolicy::Disallow,
                   QWebEngineSettings::ImageAnimationPolicy::Disallow)
/*!
    \qmlproperty enumeration WebEngineSettings::imageAnimationPolicy
    \since QtWebEngine 6.8

    Specifies how an image animation should be handled when the image frames
    are rendered for animation.

    \value WebEngineSettings.ImageAnimationPolicy.Allow
           Allows all image animations when the image frames are rendered.
    \value WebEngineSettings.ImageAnimationPolicy.AnimateOnce
           Animate the image once when the image frames are rendered.
    \value WebEngineSettings.ImageAnimationPolicy.Disallow
           Disallows all image animations when the image frames are rendered.

    Default value is \c {WebEngineSettings.ImageAnimationPolicy.Allow}.
*/
QQuickWebEngineSettings::ImageAnimationPolicy QQuickWebEngineSettings::imageAnimationPolicy() const
{
    return static_cast<QQuickWebEngineSettings::ImageAnimationPolicy>(
            d_ptr->imageAnimationPolicy());
}

ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::DisallowUnknownUrlSchemes, QWebEngineSettings::DisallowUnknownUrlSchemes)
ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::AllowUnknownUrlSchemesFromUserInteraction, QWebEngineSettings::AllowUnknownUrlSchemesFromUserInteraction)
ASSERT_ENUMS_MATCH(QQuickWebEngineSettings::AllowAllUnknownUrlSchemes, QWebEngineSettings::AllowAllUnknownUrlSchemes)

/*!
  \qmlproperty enumeration WebEngineSettings::unknownUrlSchemePolicy
  \since QtWebEngine 1.7
  Specifies how navigation requests to URLs with unknown schemes are handled.

    \value WebEngineSettings.DisallowUnknownUrlSchemes
           Disallows all navigation requests to URLs with unknown schemes.
    \value WebEngineSettings.AllowUnknownUrlSchemesFromUserInteraction
           Allows navigation requests to URLs with unknown schemes that are issued from
           user-interaction (like a mouse-click), whereas other navigation requests (for example
           from JavaScript) are suppressed.
    \value WebEngineSettings.AllowAllUnknownUrlSchemes
           Allows all navigation requests to URLs with unknown schemes.

  Default value is \c {WebEngineSettings.AllowUnknownUrlSchemesFromUserInteraction}.
*/
QQuickWebEngineSettings::UnknownUrlSchemePolicy QQuickWebEngineSettings::unknownUrlSchemePolicy() const
{
    return static_cast<QQuickWebEngineSettings::UnknownUrlSchemePolicy>(d_ptr->unknownUrlSchemePolicy());
}

void QQuickWebEngineSettings::setAutoLoadImages(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::AutoLoadImages);
    // Set unconditionally as it sets the override for the current settings while the current setting
    // could be from the fallback and is prone to changing later on.
    d_ptr->setAttribute(QWebEngineSettings::AutoLoadImages, on);
    if (wasOn != on)
        Q_EMIT autoLoadImagesChanged();
}

void QQuickWebEngineSettings::setJavascriptEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::JavascriptEnabled);
    d_ptr->setAttribute(QWebEngineSettings::JavascriptEnabled, on);
    if (wasOn != on)
        Q_EMIT javascriptEnabledChanged();
}

void QQuickWebEngineSettings::setJavascriptCanOpenWindows(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::JavascriptCanOpenWindows);
    d_ptr->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, on);
    if (wasOn != on)
        Q_EMIT javascriptCanOpenWindowsChanged();
}

void QQuickWebEngineSettings::setJavascriptCanAccessClipboard(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::JavascriptCanAccessClipboard);
    d_ptr->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, on);
    if (wasOn != on)
        Q_EMIT javascriptCanAccessClipboardChanged();
}

void QQuickWebEngineSettings::setLinksIncludedInFocusChain(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::LinksIncludedInFocusChain);
    d_ptr->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, on);
    if (wasOn != on)
        Q_EMIT linksIncludedInFocusChainChanged();
}

void QQuickWebEngineSettings::setLocalStorageEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::LocalStorageEnabled);
    d_ptr->setAttribute(QWebEngineSettings::LocalStorageEnabled, on);
    if (wasOn != on)
        Q_EMIT localStorageEnabledChanged();
}

void QQuickWebEngineSettings::setLocalContentCanAccessRemoteUrls(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls);
    d_ptr->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, on);
    if (wasOn != on)
        Q_EMIT localContentCanAccessRemoteUrlsChanged();
}


void QQuickWebEngineSettings::setSpatialNavigationEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::SpatialNavigationEnabled);
    d_ptr->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, on);
    if (wasOn != on)
        Q_EMIT spatialNavigationEnabledChanged();
}

void QQuickWebEngineSettings::setLocalContentCanAccessFileUrls(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls);
    d_ptr->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, on);
    if (wasOn != on)
        Q_EMIT localContentCanAccessFileUrlsChanged();
}

void QQuickWebEngineSettings::setHyperlinkAuditingEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::HyperlinkAuditingEnabled);
    d_ptr->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, on);
    if (wasOn != on)
        Q_EMIT hyperlinkAuditingEnabledChanged();
}

void QQuickWebEngineSettings::setErrorPageEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ErrorPageEnabled);
    d_ptr->setAttribute(QWebEngineSettings::ErrorPageEnabled, on);
    if (wasOn != on)
        Q_EMIT errorPageEnabledChanged();
}

void QQuickWebEngineSettings::setPluginsEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PluginsEnabled);
    d_ptr->setAttribute(QWebEngineSettings::PluginsEnabled, on);
    if (wasOn != on)
        Q_EMIT pluginsEnabledChanged();
}

void QQuickWebEngineSettings::setFullScreenSupportEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::FullScreenSupportEnabled);
    d_ptr->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, on);
    if (wasOn != on)
        Q_EMIT fullScreenSupportEnabledChanged();
}

void QQuickWebEngineSettings::setScreenCaptureEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ScreenCaptureEnabled);
    d_ptr->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, on);
    if (wasOn != on)
        Q_EMIT screenCaptureEnabledChanged();
}

void QQuickWebEngineSettings::setWebGLEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::WebGLEnabled);
    d_ptr->setAttribute(QWebEngineSettings::WebGLEnabled, on);
    if (wasOn != on)
        Q_EMIT webGLEnabledChanged();
}

void QQuickWebEngineSettings::setAccelerated2dCanvasEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled);
    d_ptr->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, on);
    if (wasOn != on)
        Q_EMIT accelerated2dCanvasEnabledChanged();
}

void QQuickWebEngineSettings::setAutoLoadIconsForPage(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::AutoLoadIconsForPage);
    d_ptr->setAttribute(QWebEngineSettings::AutoLoadIconsForPage, on);
    if (wasOn != on)
        Q_EMIT autoLoadIconsForPageChanged();
}

void QQuickWebEngineSettings::setTouchIconsEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::TouchIconsEnabled);
    d_ptr->setAttribute(QWebEngineSettings::TouchIconsEnabled, on);
    if (wasOn != on)
        Q_EMIT touchIconsEnabledChanged();
}

void QQuickWebEngineSettings::setPrintElementBackgrounds(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PrintElementBackgrounds);
    d_ptr->setAttribute(QWebEngineSettings::PrintElementBackgrounds, on);
    if (wasOn != on)
        Q_EMIT printElementBackgroundsChanged();
}

void QQuickWebEngineSettings::setDefaultTextEncoding(QString encoding)
{
    const QString oldDefaultTextEncoding = d_ptr->defaultTextEncoding();
    d_ptr->setDefaultTextEncoding(encoding);
    if (oldDefaultTextEncoding.compare(encoding))
        Q_EMIT defaultTextEncodingChanged();
}

void QQuickWebEngineSettings::setFocusOnNavigationEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::FocusOnNavigationEnabled);
    d_ptr->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, on);
    if (wasOn != on)
        Q_EMIT focusOnNavigationEnabledChanged();
}


void QQuickWebEngineSettings::setAllowRunningInsecureContent(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::AllowRunningInsecureContent);
    d_ptr->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, on);
    if (wasOn != on)
        Q_EMIT allowRunningInsecureContentChanged();
}

void QQuickWebEngineSettings::setAllowGeolocationOnInsecureOrigins(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins);
    d_ptr->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, on);
    if (wasOn != on)
        Q_EMIT allowGeolocationOnInsecureOriginsChanged();
}

void QQuickWebEngineSettings::setAllowWindowActivationFromJavaScript(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript);
    d_ptr->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, on);
    if (wasOn != on)
        Q_EMIT allowWindowActivationFromJavaScriptChanged();
}

void QQuickWebEngineSettings::setShowScrollBars(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ShowScrollBars);
    d_ptr->setAttribute(QWebEngineSettings::ShowScrollBars, on);
    if (wasOn != on)
        Q_EMIT showScrollBarsChanged();
}

void QQuickWebEngineSettings::setPlaybackRequiresUserGesture(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PlaybackRequiresUserGesture);
    d_ptr->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, on);
    if (wasOn != on)
        Q_EMIT playbackRequiresUserGestureChanged();
}

void QQuickWebEngineSettings::setJavascriptCanPaste(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::JavascriptCanPaste);
    d_ptr->setAttribute(QWebEngineSettings::JavascriptCanPaste, on);
    if (wasOn != on)
        Q_EMIT javascriptCanPasteChanged();
}

void QQuickWebEngineSettings::setDnsPrefetchEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::DnsPrefetchEnabled);
    d_ptr->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, on);
    if (wasOn != on)
        Q_EMIT dnsPrefetchEnabledChanged();
}

void QQuickWebEngineSettings::setPdfViewerEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PdfViewerEnabled);
    d_ptr->setAttribute(QWebEngineSettings::PdfViewerEnabled, on);
    if (wasOn != on)
        Q_EMIT pdfViewerEnabledChanged();
}

void QQuickWebEngineSettings::setNavigateOnDropEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::NavigateOnDropEnabled);
    d_ptr->setAttribute(QWebEngineSettings::NavigateOnDropEnabled, on);
    if (wasOn != on)
        Q_EMIT navigateOnDropEnabledChanged();
}

void QQuickWebEngineSettings::setReadingFromCanvasEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ReadingFromCanvasEnabled);
    d_ptr->setAttribute(QWebEngineSettings::ReadingFromCanvasEnabled, on);
    if (wasOn != on)
        Q_EMIT readingFromCanvasEnabledChanged();
}

void QQuickWebEngineSettings::setForceDarkMode(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ForceDarkMode);
    d_ptr->setAttribute(QWebEngineSettings::ForceDarkMode, on);
    if (wasOn != on)
        Q_EMIT forceDarkModeChanged();
}

void QQuickWebEngineSettings::setPrintHeaderAndFooter(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PrintHeaderAndFooter);
    d_ptr->setAttribute(QWebEngineSettings::PrintHeaderAndFooter, on);
    if (wasOn != on)
        Q_EMIT printHeaderAndFooterChanged();
}

void QQuickWebEngineSettings::setPreferCSSMarginsForPrinting(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::PreferCSSMarginsForPrinting);
    d_ptr->setAttribute(QWebEngineSettings::PreferCSSMarginsForPrinting, on);
    if (wasOn != on)
        Q_EMIT preferCSSMarginsForPrintingChanged();
}

void QQuickWebEngineSettings::setScrollAnimatorEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::ScrollAnimatorEnabled);
    d_ptr->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, on);
    if (wasOn != on)
        Q_EMIT scrollAnimatorEnabledChanged();
}

void QQuickWebEngineSettings::setUnknownUrlSchemePolicy(QQuickWebEngineSettings::UnknownUrlSchemePolicy policy)
{
    QWebEngineSettings::UnknownUrlSchemePolicy oldPolicy = d_ptr->unknownUrlSchemePolicy();
    QWebEngineSettings::UnknownUrlSchemePolicy newPolicy = static_cast<QWebEngineSettings::UnknownUrlSchemePolicy>(policy);
    d_ptr->setUnknownUrlSchemePolicy(newPolicy);
    if (oldPolicy != newPolicy)
        Q_EMIT unknownUrlSchemePolicyChanged();
}

void QQuickWebEngineSettings::setWebRTCPublicInterfacesOnly(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly);
    d_ptr->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, on);
    if (wasOn != on)
        Q_EMIT webRTCPublicInterfacesOnlyChanged();
}

void QQuickWebEngineSettings::setParentSettings(QQuickWebEngineSettings *parentSettings)
{
    d_ptr->setParentSettings(parentSettings->d_ptr.data());
}

void QQuickWebEngineSettings::setImageAnimationPolicy(
        QQuickWebEngineSettings::ImageAnimationPolicy policy)
{
    QWebEngineSettings::ImageAnimationPolicy oldPolicy = d_ptr->imageAnimationPolicy();
    QWebEngineSettings::ImageAnimationPolicy newPolicy =
            static_cast<QWebEngineSettings::ImageAnimationPolicy>(policy);
    d_ptr->setImageAnimationPolicy(newPolicy);
    if (oldPolicy != newPolicy)
        Q_EMIT imageAnimationPolicyChanged();
}

void QQuickWebEngineSettings::setTouchEventsApiEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::TouchEventsApiEnabled);
    d_ptr->setAttribute(QWebEngineSettings::TouchEventsApiEnabled, on);
    if (wasOn != on)
        Q_EMIT touchEventsApiEnabledChanged();
}

void QQuickWebEngineSettings::setBackForwardCacheEnabled(bool on)
{
    bool wasOn = d_ptr->testAttribute(QWebEngineSettings::BackForwardCacheEnabled);
    d_ptr->setAttribute(QWebEngineSettings::BackForwardCacheEnabled, on);
    if (wasOn != on)
        Q_EMIT backForwardCacheEnabledChanged();
}

QT_END_NAMESPACE

#include "moc_qquickwebenginesettings_p.cpp"
