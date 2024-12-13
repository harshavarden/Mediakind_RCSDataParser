/*
 * RCSDataParser.hpp
 *
 * Created on: Dec 20, 2023
 *
 * Copyright (c) Mediakind, Inc.
 */

#ifndef RCSDATAPARSER_HPP_
#define RCSDATAPARSER_HPP_

#include <boost/serialization/variant.hpp>
#include <boost/variant.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/atomic.hpp>
#include <set>
#include <map>
#include <variant>

using namespace std;
using boost::property_tree::ptree;

//key should be enum of type RcsKeyData
enum RcsKeyData{
    PERIODIC_RCS_REFRESH_ENABLED, // 0
    PERIODIC_RCS_REFRESH_INTERVAL,
    USE_FCGI_BUILTIN_CASSANDRA,
    LOW_LATENCY_ENABLED,
    LOW_LATENCY_SERVICES,
    LOG_SESSION_DATA_WHEN_FAILED,
    WV_SECURITY_LEVELS_SUFFIX,
    EVICTION_LIVE_RIGHTS_URL,
    EVICTION_VOD_RIGHTS_URL,
    PREFER_STS_DEVICE_ID,
    SESSION_ENFORCEMENT_FOR_LICENSE,
    USE_LIGHT_RIGHTS_FOR_GUEST_USERS,
    RDC_SCHEDULER_ENABLED,
    PREFER_REDIS_KEY_SEARCH,
    SKIP_REGISTRATION_FOR_GUEST,
    DO_CLIENT_RIGHTS_TABLE_QUERY,
    DO_MEDIA_RIGHTS_TABLE_QUERY,
    DO_MEDIA_GROUP_RIGHTS_TABLE_QUERY,
    DO_PLATFORM_RIGHTS_TABLE_QUERY,
    DO_PLATFORM_GROUP_RIGHTS_TABLE_QUERY,
    CASSANDRA_CACHING_REFRESH_INTERVAL,
    REGISTRATION_TABLE_TTL,
    REGISTRATION_IN_ROLL_ENABLED,
    ACCOUNT_GROUP_UPDATE_ENABLED,
    DEPRECATED_API_IMC_LIST,
    DEFAULT_INHOME_STATUS,
    LOCAL_RIGHTS_CACHE_ENABLED,
    SUBSCRIBER_RIGHTS_CACHE_ENABLED,
    ANALYTICS_PERCENT_CONFIG,
    RIGHTS_REQUEST_TIMEOUT_S,
    LICENSE_REQUEST_TIMEOUT_S,
    PLAYREADY_ERROR_ON_RIGHTS_FAILURE,
    START_END_TIME_ROUND_OFF_S,
    RDC_SCHED_INTERVAL_SEC,
    PREGAME_START_OFFSET_M,
    ANALYTICS_SCHED_INTERVAL_SEC,
    PSQL_HOSTADDR,
    PSQL_PORT,
    PSQL_USERNAME,
    PSQL_PASSWORD,
    IGNORE_AUDIO_KEY_ID_IN_PSSH
};

//array to hold actual keys
static const char* RcsDataKeysNames[] = {
    "PeriodicRCSRefreshEnabled", // 0
    "PeriodicRCSRefreshInterval",
    "UseFcgiBuiltInCassandra",
    "LowLatencyEnabled",
    "LowLatencyServices",
    "LogSessionDataWhenFailed",
    "WVSecurityLevelsSuffix",
    "AzukiEvictionLiveRightsURL",
    "AzukiEvictionVodRightsURL",
    "PreferSTSDeviceId",
    "SessionEnforcementForLicense",
    "UseLightRightsApiForGuestUsers",
    "RdcSchedulerEnabled",
    "PreferRedisKeySearch",
    "SkipRegistrationForGuest",
    "DoClientRightsTableQuery",
    "DoMediaRightsTableQuery",
    "DoMediaGroupRightsTableQuery",
    "DoPlatformRightsTableQuery",
    "DoPlatformGroupRightsTableQuery",
    "CassandraCachingRefreshInterval",
    "RegistrationTableTtl",
    "RegistrationInRollEnabled",
    "AccountGroupUpdateEnabled",
    "DeprecatedApiImcList",
    "DefaultInhomeStatus",
    "LocalRightsCacheEnabled",
    "SubscriberRightsCacheEnabled",
    "AnalyticsPercentConfig",
    "RightsRequestTimeout_s",
    "LicenseRequestTimeout_s",
    "PlayreadyErrorOnRightsFailure",
    "StartEndTimeRoundOff_s",
    "RdcSchedIntervalSec",
    "PregameStartOffset_m",
    "AnalyticsSchedIntervalSec",
    "PsqlHostaddr",
    "PsqlPort",
    "PsqlUsername",
    "PsqlPassword",
    "IgnoreAudioKeyidInPSSH"
};

//set to hold boolean type keys
static set <string> boolTypeKey = {"PeriodicRCSRefreshEnabled",
                                   "LowLatencyEnabled",
                                   "UseFcgiBuiltInCassandra",
                                   "PreferSTSDeviceId",
                                   "SessionEnforcementForLicense",
                                   "UseLightRightsApiForGuestUsers",
                                   "RdcSchedulerEnabled",
                                   "PreferRedisKeySearch",
                                   "SkipRegistrationForGuest",
                                   "DoClientRightsTableQuery",
                                   "DoMediaRightsTableQuery",
                                   "DoMediaGroupRightsTableQuery",
                                   "DoPlatformRightsTableQuery",
                                   "DoPlatformGroupRightsTableQuery",
                                   "RegistrationInRollEnabled",
                                   "AccountGroupUpdateEnabled",
                                   "LocalRightsCacheEnabled",
                                   "SubscriberRightsCacheEnabled",
                                   "PlayreadyErrorOnRightsFailure",
                                   "IgnoreAudioKeyidInPSSH"
};

//set to hold string type keys
static set <string> stringTypeKey = {"LowLatencyServices",
                                     "LogSessionDataWhenFailed",
                                     "WVSecurityLevelsSuffix",
                                     "AzukiEvictionLiveRightsURL",
                                     "AzukiEvictionVodRightsURL",
                                     "RegistrationTableTtl",
                                     "DeprecatedApiImcList",
                                     "DefaultInhomeStatus",
                                     "AnalyticsPercentConfig",
                                     "PsqlHostaddr",
                                     "PsqlUsername",
                                     "PsqlPassword"
};

//set to hold int type keys
static set <string> intTypeKey = {"PeriodicRCSRefreshInterval",
                                  "CassandraCachingRefreshInterval", // The value should be defined in seconds.By default value is hash defined as 300 sec                                 
                                  "RightsRequestTimeout_s",  // HttpV1Rights and license(only for playready and widevine) timeout should be defined in seconds.By default value is hash defined as 9 sec
                                  "LicenseRequestTimeout_s",
                                  "StartEndTimeRoundOff_s", // Default value is 60 sec, and roundoff could disabled with 0 sec
                                  "RdcSchedIntervalSec",
                                  "PregameStartOffset_m",
                                  "AnalyticsSchedIntervalSec",
                                  "CassandraCachingRefreshInterval", // The value should be defined in seconds.By default value is hash defined as 300 sec 
                                  "PsqlPort"
};

class RCSDataParser {
public:
    RCSDataParser();
    ~RCSDataParser();
    void InitializeRCSDataParser(string& rcsConfigFile);
    
    typedef boost::variant<bool, string, int> RCSValue;
    typedef map<string, boost::variant<bool, string, int>> RCSMap; // map to get rcs value.

    static RCSDataParser* getRCSInstance();
    static void GetBoolRcsConfig(RcsKeyData key, bool &value);
    static void GetStringRcsConfig(RcsKeyData key, string &value);
    static void GetIntRcsConfig(RcsKeyData key, int &value);
    static void GetPtreeRcsConfig(RcsKeyData key, ptree &pt);

    static void SetBoolRcsConfig(RcsKeyData key, const string &value);
    static void SetStringRcsConfig(RcsKeyData key, const string &value);
    static void SetIntRcsConfig(RcsKeyData key, const string &value);

private:

    RCSMap m_rcsmap;
    static RCSDataParser *m_rcsptr;

    void insertBoolDataToRCSMap(const string& key, const string& value);
    void insertStringDataToRCSMap(const string& key, const string& value);
    void insertIntDataToRCSMap(const string& key, const string& value);
    bool getDataFromRCSMap(const string &key, RCSValue& value);
};

#endif /*RCSDATAPARSER_HPP_*/
