// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

// originally based on android_webview/browser/network_service/aw_proxying_restricted_cookie_manager.cc:
// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "proxying_restricted_cookie_manager_qt.h"

#include "api/qwebenginecookiestore.h"
#include "api/qwebenginecookiestore_p.h"
#include "profile_io_data_qt.h"
#include "type_conversion.h"

#include "base/memory/ptr_util.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"

namespace QtWebEngineCore {

// static
void ProxyingRestrictedCookieManagerQt::CreateAndBind(ProfileIODataQt *profileIoData,
                                                      mojo::PendingRemote<network::mojom::RestrictedCookieManager> underlying_rcm,
                                                      mojo::PendingReceiver<network::mojom::RestrictedCookieManager> receiver)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

    content::GetIOThreadTaskRunner({})->PostTask(FROM_HERE,
                   base::BindOnce(&ProxyingRestrictedCookieManagerQt::CreateAndBindOnIoThread,
                                  profileIoData,
                                  std::move(underlying_rcm),
                                  std::move(receiver)));
}


// static
void ProxyingRestrictedCookieManagerQt::CreateAndBindOnIoThread(ProfileIODataQt *profileIoData,
                                                                mojo::PendingRemote<network::mojom::RestrictedCookieManager> underlying_rcm,
                                                                mojo::PendingReceiver<network::mojom::RestrictedCookieManager> receiver)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

    auto wrapper = base::WrapUnique(new ProxyingRestrictedCookieManagerQt(
                                        profileIoData->getWeakPtrOnIOThread(),
                                        std::move(underlying_rcm)));
    mojo::MakeSelfOwnedReceiver(std::move(wrapper), std::move(receiver));
}

ProxyingRestrictedCookieManagerQt::ProxyingRestrictedCookieManagerQt(
        base::WeakPtr<ProfileIODataQt> profileIoData,
        mojo::PendingRemote<network::mojom::RestrictedCookieManager> underlyingRestrictedCookieManager)
        : m_profileIoData(std::move(profileIoData))
        , underlying_restricted_cookie_manager_(std::move(underlyingRestrictedCookieManager))
        , weak_factory_(this)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
}

ProxyingRestrictedCookieManagerQt::~ProxyingRestrictedCookieManagerQt()
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
}

void ProxyingRestrictedCookieManagerQt::GetAllForUrl(const GURL &url,
                                                     const net::SiteForCookies &site_for_cookies,
                                                     const url::Origin &top_frame_origin,
                                                     net::StorageAccessApiStatus storage_access_api_status,
                                                     network::mojom::CookieManagerGetOptionsPtr options,
                                                     bool is_ad_tagged,
                                                     bool force_disable_third_party_cookies,
                                                     GetAllForUrlCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

    if (allowCookies(url, site_for_cookies)) {
        underlying_restricted_cookie_manager_->GetAllForUrl(url, site_for_cookies, top_frame_origin, storage_access_api_status,
                                                            std::move(options), is_ad_tagged, force_disable_third_party_cookies, std::move(callback));
    } else {
        std::move(callback).Run(std::vector<net::CookieWithAccessResult>());
    }
}

void ProxyingRestrictedCookieManagerQt::SetCanonicalCookie(const net::CanonicalCookie &cookie,
                                                           const GURL &url,
                                                           const net::SiteForCookies &site_for_cookies,
                                                           const url::Origin &top_frame_origin,
                                                           net::StorageAccessApiStatus storage_access_api_status,
                                                           net::CookieInclusionStatus status,
                                                           SetCanonicalCookieCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

    if (allowCookies(url, site_for_cookies)) {
        underlying_restricted_cookie_manager_->SetCanonicalCookie(cookie, url, site_for_cookies, top_frame_origin,
                                                                  storage_access_api_status, status, std::move(callback));
    } else {
        std::move(callback).Run(false);
    }
}

void ProxyingRestrictedCookieManagerQt::AddChangeListener(const GURL &url,
                                                          const net::SiteForCookies &site_for_cookies,
                                                          const url::Origin &top_frame_origin,
                                                          net::StorageAccessApiStatus storage_access_api_status,
                                                          mojo::PendingRemote<network::mojom::CookieChangeListener> listener,
                                                          AddChangeListenerCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
    underlying_restricted_cookie_manager_->AddChangeListener(url, site_for_cookies, top_frame_origin, storage_access_api_status,
                                                             std::move(listener), std::move(callback));
}

void ProxyingRestrictedCookieManagerQt::SetCookieFromString(const GURL &url,
                                                            const net::SiteForCookies &site_for_cookies,
                                                            const url::Origin &top_frame_origin,
                                                            net::StorageAccessApiStatus storage_access_api_status,
                                                            const std::string &cookie,
                                                            SetCookieFromStringCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

    if (allowCookies(url, site_for_cookies)) {
        underlying_restricted_cookie_manager_->SetCookieFromString(url, site_for_cookies, top_frame_origin, storage_access_api_status,
                                                                   cookie, std::move(callback));
    } else {
        std::move(callback).Run();
    }
}

void ProxyingRestrictedCookieManagerQt::GetCookiesString(const GURL &url,
                                                         const net::SiteForCookies &site_for_cookies,
                                                         const url::Origin &top_frame_origin,
                                                         net::StorageAccessApiStatus storage_access_api_status,
                                                         bool get_version_shared_memory,
                                                         bool is_ad_tagged,
                                                         bool force_disable_third_party_cookies,
                                                         GetCookiesStringCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

    if (allowCookies(url, site_for_cookies)) {
        underlying_restricted_cookie_manager_->GetCookiesString(url, site_for_cookies, top_frame_origin,
                                                                storage_access_api_status, get_version_shared_memory,
                                                                is_ad_tagged, force_disable_third_party_cookies,
                                                                std::move(callback));
    } else {
        std::move(callback).Run(network::mojom::kInvalidCookieVersion, base::ReadOnlySharedMemoryRegion(), "");
    }
}

void ProxyingRestrictedCookieManagerQt::CookiesEnabledFor(const GURL &url,
                                                          const net::SiteForCookies &site_for_cookies,
                                                          const url::Origin & /*top_frame_origin*/,
                                                          net::StorageAccessApiStatus /*storage_access_api_status*/,
                                                          CookiesEnabledForCallback callback)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
    std::move(callback).Run(allowCookies(url, site_for_cookies));
}

bool ProxyingRestrictedCookieManagerQt::allowCookies(const GURL &url, const net::SiteForCookies &site_for_cookies) const
{
    if (!m_profileIoData)
        return false;
    return m_profileIoData->canGetCookies(toQt(site_for_cookies.first_party_url()), toQt(url));
}

}  // namespace QtWebEngineCore
