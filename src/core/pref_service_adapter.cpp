// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "pref_service_adapter.h"

#include "profile_adapter.h"
#include "type_conversion.h"
#include "web_engine_context.h"
#include "web_engine_library_info.h"

#include "base/base_paths.h"
#include "base/threading/thread_restrictions.h"
#include "chrome/browser/prefs/chrome_command_line_pref_store.h"
#include "content/public/browser/browser_thread.h"
#include "components/autofill/core/common/autofill_prefs.h"
#include "components/language/core/browser/pref_names.h"
#include "components/prefs/pref_member.h"
#include "components/prefs/in_memory_pref_store.h"
#include "components/prefs/json_pref_store.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/pref_service_factory.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/signin/internal/identity_manager/account_tracker_service.h"
#include "components/signin/internal/identity_manager/gaia_cookie_manager_service.h"
#include "components/signin/internal/identity_manager/primary_account_manager.h"
#include "components/signin/internal/identity_manager/profile_oauth2_token_service.h"
#include "components/user_prefs/user_prefs.h"
#include "components/proxy_config/pref_proxy_config_tracker_impl.h"
#include "chrome/browser/gcm/gcm_product_util.h"
#include "chrome/common/pref_names.h"
#include "extensions/buildflags/buildflags.h"
#include "content/public/browser/browser_context.h"

#include "components/pref_registry/pref_registry_syncable.h"
#include "chrome/browser/devtools/devtools_settings.h"

#if QT_CONFIG(webengine_spellchecker)
#include "chrome/browser/spellchecker/spellcheck_service.h"
#include "components/spellcheck/browser/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "components/guest_view/browser/guest_view_manager.h"
#include "extensions/browser/extension_protocols.h"
#include "extensions/browser/pref_names.h"
#include "extensions/browser/process_manager.h"
#include "extensions/common/constants.h"
#endif

#if defined(Q_OS_WIN)
#include "components/os_crypt/sync/os_crypt.h"
#endif

namespace {
static const char kPrefMediaDeviceIDSalt[] = "qtwebengine.media_device_salt_id";
}

using namespace Qt::StringLiterals;

namespace QtWebEngineCore {

void PrefServiceAdapter::setup(const ProfileAdapter &profileAdapter)
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    PrefServiceFactory factory;
    factory.set_command_line_prefs(base::MakeRefCounted<ChromeCommandLinePrefStore>(
            base::CommandLine::ForCurrentProcess()));

    QString userPrefStorePath;
    userPrefStorePath += profileAdapter.dataPath();
    if (!profileAdapter.isOffTheRecord() && !userPrefStorePath.isEmpty() &&
            const_cast<ProfileAdapter *>(&profileAdapter)->ensureDataPathExists()) {
        userPrefStorePath += QDir::separator();
        userPrefStorePath += "user_prefs.json"_L1;
        factory.set_user_prefs(base::MakeRefCounted<JsonPrefStore>(toFilePath(userPrefStorePath)));
    } else {
        factory.set_user_prefs(new InMemoryPrefStore);
    }

    auto registry = base::MakeRefCounted<PrefRegistrySimple>();
    PrefProxyConfigTrackerImpl::RegisterPrefs(registry.get());

#if QT_CONFIG(webengine_spellchecker)
    // Initial spellcheck settings
    registry->RegisterStringPref(language::prefs::kAcceptLanguages, std::string());
    registry->RegisterListPref(spellcheck::prefs::kSpellCheckDictionaries);
    registry->RegisterListPref(spellcheck::prefs::kSpellCheckForcedDictionaries);
    registry->RegisterListPref(spellcheck::prefs::kSpellCheckBlocklistedDictionaries);
    registry->RegisterStringPref(spellcheck::prefs::kSpellCheckDictionary, std::string());
    registry->RegisterBooleanPref(spellcheck::prefs::kSpellCheckEnable, false);
    registry->RegisterBooleanPref(spellcheck::prefs::kSpellCheckUseSpellingService, false);
#endif // QT_CONFIG(webengine_spellchecker)
    registry->RegisterBooleanPref(prefs::kAccessibilityImageLabelsEnabled, false);

    // chrome/browser/notifications
    registry->RegisterIntegerPref(prefs::kNotificationNextPersistentId, 10000);
    // chrome/browser/push_messaging
    registry->RegisterDictionaryPref(prefs::kPushMessagingAppIdentifierMap);
    // chrome/browser/gcm
    gcm::RegisterPrefs(registry.get());

    // signin
    PrimaryAccountManager::RegisterProfilePrefs(registry.get());
    ProfileOAuth2TokenService::RegisterProfilePrefs(registry.get());
    GaiaCookieManagerService::RegisterPrefs(registry.get());
    AccountTrackerService::RegisterPrefs(registry.get());

#if defined(Q_OS_WIN)
    OSCrypt::RegisterLocalPrefs(registry.get());
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
    registry->RegisterDictionaryPref(extensions::pref_names::kExtensions);
    registry->RegisterListPref(extensions::pref_names::kInstallAllowList);
    registry->RegisterListPref(extensions::pref_names::kInstallDenyList);
    registry->RegisterDictionaryPref(extensions::pref_names::kInstallForceList);
    registry->RegisterListPref(extensions::pref_names::kAllowedTypes);
    registry->RegisterBooleanPref(extensions::pref_names::kStorageGarbageCollect, false);
    registry->RegisterListPref(extensions::pref_names::kAllowedInstallSites);
    registry->RegisterStringPref(extensions::pref_names::kLastChromeVersion, std::string());
    registry->RegisterListPref(extensions::pref_names::kNativeMessagingBlocklist);
    registry->RegisterListPref(extensions::pref_names::kNativeMessagingAllowlist);
    registry->RegisterBooleanPref(extensions::pref_names::kNativeMessagingUserLevelHosts, true);
    registry->RegisterListPref(extensions::pref_names::kExtendedBackgroundLifetimeForPortConnectionsToUrls);
#endif // BUILDFLAG(ENABLE_EXTENSIONS)

    // Media device salt id key
    // Can't be a random value since every time we run the setup code the
    // default value will be different. We'll need to initialize it later.
    registry->RegisterStringPref(kPrefMediaDeviceIDSalt, std::string());
    registry->RegisterStringPref(prefs::kShownAccessibilityApiType, "qt");

    registry->RegisterBooleanPref(autofill::prefs::kAutofillEnabledDeprecated, false);
    registry->RegisterBooleanPref(autofill::prefs::kAutofillProfileEnabled, false);
    registry->RegisterBooleanPref(autofill::prefs::kAutofillCreditCardEnabled, false);
    registry->RegisterBooleanPref(autofill::prefs::kAutofillCreditCardFidoAuthEnabled, false);

    // devtools
    registry->RegisterDictionaryPref(prefs::kDevToolsFileSystemPaths);
    registry->RegisterDictionaryPref(prefs::kDevToolsEditedFiles);
    registry->RegisterDictionaryPref(prefs::kDevToolsPreferences);
    registry->RegisterBooleanPref(prefs::kDevToolsSyncPreferences, false);
    // even if kDevToolsSyncPreferences is disabled, the js frontend tries to access
    // these two. E.g.: 'clearPreferences', that is overridden by devtools_compatibility.js
    registry->RegisterDictionaryPref(prefs::kDevToolsSyncedPreferencesSyncDisabled);
    registry->RegisterDictionaryPref(prefs::kDevToolsSyncedPreferencesSyncEnabled);

    {
        base::ScopedAllowBlocking allowBlock;
        m_prefService = factory.Create(registry);
    }

#if QT_CONFIG(webengine_spellchecker)
    // Ignore stored values for these options to preserve backwards compatibility.
    m_prefService->ClearPref(spellcheck::prefs::kSpellCheckEnable);
    m_prefService->ClearPref(spellcheck::prefs::kSpellCheckDictionaries);
#endif // QT_CONFIG(webengine_spellchecker)

    m_prefService->SchedulePendingLossyWrites();
}

void PrefServiceAdapter::commit()
{
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    // Make sure modified preferences are written to disk
    m_prefService->CommitPendingWrite();
}

PrefService* PrefServiceAdapter::prefService()
{
    return m_prefService.get();
}

const PrefService* PrefServiceAdapter::prefService() const
{
    return m_prefService.get();
}

std::string PrefServiceAdapter::mediaDeviceIdSalt() const
{
    return m_prefService->GetString(kPrefMediaDeviceIDSalt);
}

#if QT_CONFIG(webengine_spellchecker)

void PrefServiceAdapter::setSpellCheckLanguages(const QStringList &languages)
{
    StringListPrefMember dictionaries_pref;
    dictionaries_pref.Init(spellcheck::prefs::kSpellCheckDictionaries, m_prefService.get());
    std::vector<std::string> dictionaries;
    dictionaries.reserve(languages.size());
    for (const auto &language : languages)
        dictionaries.push_back(language.toStdString());
    dictionaries_pref.SetValue(dictionaries);
    m_prefService->SchedulePendingLossyWrites();
}

QStringList PrefServiceAdapter::spellCheckLanguages() const
{
    QStringList spellcheck_dictionaries;
    const auto &list = m_prefService->GetList(spellcheck::prefs::kSpellCheckDictionaries);
    for (const auto &dictionary : list) {
        spellcheck_dictionaries.append(QString::fromStdString(dictionary.GetString()));
    }

    return spellcheck_dictionaries;
}

void PrefServiceAdapter::setSpellCheckEnabled(bool enabled)
{
    if (enabled == m_prefService->GetBoolean(spellcheck::prefs::kSpellCheckEnable))
        return;

    if (!WebEngineLibraryInfo::getPath(base::DIR_APP_DICTIONARIES, enabled).empty()) {
        m_prefService->SetBoolean(spellcheck::prefs::kSpellCheckEnable, enabled);
        m_prefService->SchedulePendingLossyWrites();
    }
}

bool PrefServiceAdapter::isSpellCheckEnabled() const
{
    return m_prefService->GetBoolean(spellcheck::prefs::kSpellCheckEnable);
}

#endif // QT_CONFIG(webengine_spellchecker)
}
