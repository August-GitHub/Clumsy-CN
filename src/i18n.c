#include "i18n.h"
#include "common.h"
#include <string.h>

static Language currentLang = LANG_ENGLISH;

static const char* strings[LANG_COUNT][STR_COUNT_MAX] = {
    [LANG_ENGLISH] = {
        [STR_TITLE] = "clumsy " CLUMSY_VERSION,
        [STR_FILTERING] = "Filtering",
        [STR_FUNCTIONS] = "Functions",
        [STR_START] = "Start",
        [STR_STOP] = "Stop",
        [STR_PRESETS] = "Presets:  ",
        [STR_NOTICE] = "NOTICE: When capturing localhost (loopback) packets, you CAN'T include inbound criteria.\n"
                        "Filters like 'udp' need to be 'udp and outbound' to work. See readme for more info.",
        [STR_STARTED] = "Started filtering. Enable functionalities to take effect.",
        [STR_STOPPED] = "Stopped. To begin again, edit criteria and click Start.",
        [STR_ABORTING] = "Aborting",
        [STR_ALREADY_RUNNING] = "Theres' already an instance of clumsy running.",
        [STR_32BIT_ON_64BIT] = "You're running 32bit clumsy on 64bit Windows, which wouldn't work. Please use the 64bit clumsy version.",
        [STR_UNSUPPORTED_WINDOWS] = "Unsupported Windows version. clumsy only supports Windows Vista or above.",
        [STR_NEED_ELEVATION] = "clumsy needs to be elevated to work. Run as Administrator or click Yes in promoted UAC dialog",
        [STR_FAILED_GET_PATH] = "Failed to get clumsy path. Please place the executable in a normal directory.",
        [STR_LAG] = "Lag",
        [STR_DROP] = "Drop",
        [STR_THROTTLE] = "Throttle",
        [STR_DUPLICATE] = "Duplicate",
        [STR_OUT_OF_ORDER] = "Out of order",
        [STR_TAMPER] = "Tamper",
        [STR_SET_TCP_RST] = "Set TCP RST",
        [STR_BANDWIDTH] = "Bandwidth",
        [STR_INBOUND] = "Inbound",
        [STR_OUTBOUND] = "Outbound",
        [STR_DELAY_MS] = "Delay(ms):",
        [STR_CHANCE_PERCENT] = "Chance(%):",
        [STR_COUNT] = "Count:",
        [STR_TIMEFRAME_MS] = "Timeframe(ms):",
        [STR_DROP_THROTTLED] = "Drop Throttled",
        [STR_REDO_CHECKSUM] = "Redo Checksum",
        [STR_RST_NEXT_PACKET] = "RST next packet",
        [STR_LIMIT_KB_S] = "Limit(KB/s):",
        [STR_LANGUAGE] = "Language:",
        [STR_ENGLISH] = "English",
        [STR_CHINESE_SIMPLIFIED] = "简体中文",
        [STR_CHINESE_TRADITIONAL] = "繁體中文",
        [STR_JAPANESE] = "日本語",
        [STR_LOOPBACK_PACKETS] = "loopback packets"
    },
    [LANG_CHINESE_SIMPLIFIED] = {
        [STR_TITLE] = "clumsy " CLUMSY_VERSION,
        [STR_FILTERING] = "过滤规则",
        [STR_FUNCTIONS] = "功能",
        [STR_START] = "开始",
        [STR_STOP] = "停止",
        [STR_PRESETS] = "预设:  ",
        [STR_NOTICE] = "注意：当捕获本地（环回）数据包时，不能包含入站条件。\n"
                        "像 'udp' 这样的过滤器需要改为 'udp and outbound' 才能工作。请参阅 readme 了解更多信息。",
        [STR_STARTED] = "已开始过滤。启用功能以生效。",
        [STR_STOPPED] = "已停止。要重新开始，请编辑条件并点击“开始”。",
        [STR_ABORTING] = "正在中止",
        [STR_ALREADY_RUNNING] = "clumsy 已经有一个实例正在运行。",
        [STR_32BIT_ON_64BIT] = "您正在 64 位 Windows 上运行 32 位的 clumsy，这将无法工作。请使用 64 位版本的 clumsy。",
        [STR_UNSUPPORTED_WINDOWS] = "不支持的 Windows 版本。clumsy 仅支持 Windows Vista 或更高版本。",
        [STR_NEED_ELEVATION] = "clumsy 需要提升权限才能工作。以管理员身份运行或在 UAC 对话框中点击“是”。",
        [STR_FAILED_GET_PATH] = "无法获取 clumsy 路径。请将可执行文件放在正常目录中。",
        [STR_LAG] = "延迟",
        [STR_DROP] = "丢包",
        [STR_THROTTLE] = "节流",
        [STR_DUPLICATE] = "复制",
        [STR_OUT_OF_ORDER] = "乱序",
        [STR_TAMPER] = "篡改",
        [STR_SET_TCP_RST] = "设置 TCP RST",
        [STR_BANDWIDTH] = "带宽",
        [STR_INBOUND] = "入站",
        [STR_OUTBOUND] = "出站",
        [STR_DELAY_MS] = "延迟(ms):",
        [STR_CHANCE_PERCENT] = "概率(%):",
        [STR_COUNT] = "数量:",
        [STR_TIMEFRAME_MS] = "时间窗口(ms):",
        [STR_DROP_THROTTLED] = "丢弃节流包",
        [STR_REDO_CHECKSUM] = "重新计算校验和",
        [STR_RST_NEXT_PACKET] = "重置下一个包",
        [STR_LIMIT_KB_S] = "限制(KB/s):",
        [STR_LANGUAGE] = "语言:",
        [STR_ENGLISH] = "English",
        [STR_CHINESE_SIMPLIFIED] = "简体中文",
        [STR_CHINESE_TRADITIONAL] = "繁體中文",
        [STR_JAPANESE] = "日本語",
        [STR_LOOPBACK_PACKETS] = "本地回环数据包"
    },
    [LANG_CHINESE_TRADITIONAL] = {
        [STR_TITLE] = "clumsy " CLUMSY_VERSION,
        [STR_FILTERING] = "過濾規則",
        [STR_FUNCTIONS] = "功能",
        [STR_START] = "開始",
        [STR_STOP] = "停止",
        [STR_PRESETS] = "預設:  ",
        [STR_NOTICE] = "注意：當擷取本機（迴圈）封包時，不能包含入站條件。\n"
                        "像 'udp' 這樣的過濾器需要改為 'udp and outbound' 才能工作。請參閱 readme 了解更多資訊。",
        [STR_STARTED] = "已開始過濾。啟用功能以生效。",
        [STR_STOPPED] = "已停止。要重新開始，請編輯條件並點擊「開始」。",
        [STR_ABORTING] = "正在中止",
        [STR_ALREADY_RUNNING] = "clumsy 已經有一個執行個體正在執行。",
        [STR_32BIT_ON_64BIT] = "您正在 64 位元 Windows 上執行 32 位元的 clumsy，這將無法工作。請使用 64 位元版本的 clumsy。",
        [STR_UNSUPPORTED_WINDOWS] = "不支援的 Windows 版本。clumsy 僅支援 Windows Vista 或更高版本。",
        [STR_NEED_ELEVATION] = "clumsy 需要提升權限才能工作。以系統管理員身分執行或在 UAC 對話方塊中點擊「是」。",
        [STR_FAILED_GET_PATH] = "無法取得 clumsy 路徑。請將可執行檔放在正常目錄中。",
        [STR_LAG] = "延遲",
        [STR_DROP] = "丟包",
        [STR_THROTTLE] = "節流",
        [STR_DUPLICATE] = "複製",
        [STR_OUT_OF_ORDER] = "亂序",
        [STR_TAMPER] = "竄改",
        [STR_SET_TCP_RST] = "設定 TCP RST",
        [STR_BANDWIDTH] = "頻寬",
        [STR_INBOUND] = "入站",
        [STR_OUTBOUND] = "出站",
        [STR_DELAY_MS] = "延遲(ms):",
        [STR_CHANCE_PERCENT] = "機率(%):",
        [STR_COUNT] = "數量:",
        [STR_TIMEFRAME_MS] = "時間視窗(ms):",
        [STR_DROP_THROTTLED] = "丟棄節流封包",
        [STR_REDO_CHECKSUM] = "重新計算校驗和",
        [STR_RST_NEXT_PACKET] = "重設下一個封包",
        [STR_LIMIT_KB_S] = "限制(KB/s):",
        [STR_LANGUAGE] = "語言:",
        [STR_ENGLISH] = "English",
        [STR_CHINESE_SIMPLIFIED] = "简体中文",
        [STR_CHINESE_TRADITIONAL] = "繁體中文",
        [STR_JAPANESE] = "日本語",
        [STR_LOOPBACK_PACKETS] = "本地迴圈封包"
    },
    [LANG_JAPANESE] = {
        [STR_TITLE] = "clumsy " CLUMSY_VERSION,
        [STR_FILTERING] = "フィルター",
        [STR_FUNCTIONS] = "機能",
        [STR_START] = "開始",
        [STR_STOP] = "停止",
        [STR_PRESETS] = "プリセット:  ",
        [STR_NOTICE] = "注意：ローカルホスト（ループバック）パケットをキャプチャする場合、インバウンド基準を含めることはできません。\n"
                        "'udp' のようなフィルターは 'udp and outbound' とする必要があります。詳細は readme を参照してください。",
        [STR_STARTED] = "フィルタリングを開始しました。機能を有効にしてください。",
        [STR_STOPPED] = "停止しました。再開するには、条件を編集して「開始」をクリックしてください。",
        [STR_ABORTING] = "中止中",
        [STR_ALREADY_RUNNING] = "clumsy は既に実行中です。",
        [STR_32BIT_ON_64BIT] = "64ビットWindowsで32ビット版のclumsyを実行しています。これは動作しません。64ビット版のclumsyを使用してください。",
        [STR_UNSUPPORTED_WINDOWS] = "サポートされていないWindowsバージョンです。clumsyはWindows Vista以降のみサポートしています。",
        [STR_NEED_ELEVATION] = "clumsyは管理者権限が必要です。管理者として実行するか、UACダイアログで「はい」をクリックしてください。",
        [STR_FAILED_GET_PATH] = "clumsyのパスを取得できませんでした。実行ファイルを通常のディレクトリに配置してください。",
        [STR_LAG] = "遅延",
        [STR_DROP] = "ドロップ",
        [STR_THROTTLE] = "スロットル",
        [STR_DUPLICATE] = "複製",
        [STR_OUT_OF_ORDER] = "順序変更",
        [STR_TAMPER] = "改ざん",
        [STR_SET_TCP_RST] = "TCP RST設定",
        [STR_BANDWIDTH] = "帯域幅",
        [STR_INBOUND] = "インバウンド",
        [STR_OUTBOUND] = "アウトバウンド",
        [STR_DELAY_MS] = "遅延(ms):",
        [STR_CHANCE_PERCENT] = "確率(%):",
        [STR_COUNT] = "数:",
        [STR_TIMEFRAME_MS] = "時間枠(ms):",
        [STR_DROP_THROTTLED] = "スロットルパケットをドロップ",
        [STR_REDO_CHECKSUM] = "チェックサム再計算",
        [STR_RST_NEXT_PACKET] = "次のパケットをRST",
        [STR_LIMIT_KB_S] = "制限(KB/s):",
        [STR_LANGUAGE] = "言語:",
        [STR_ENGLISH] = "English",
        [STR_CHINESE_SIMPLIFIED] = "简体中文",
        [STR_CHINESE_TRADITIONAL] = "繁體中文",
        [STR_JAPANESE] = "日本語",
        [STR_LOOPBACK_PACKETS] = "ローカルホストパケット"
    }
};

void i18nInit(Language lang) {
    if (lang < 0 || lang >= LANG_COUNT) {
        lang = LANG_ENGLISH;
    }
    currentLang = lang;
}

Language i18nGetCurrentLanguage(void) {
    return currentLang;
}

void i18nSetLanguage(Language lang) {
    if (lang >= 0 && lang < LANG_COUNT) {
        currentLang = lang;
    }
}

const char* i18nGetString(StringID id) {
    if (id < 0 || id >= STR_COUNT_MAX) {
        return "";
    }
    const char* str = strings[currentLang][id];
    return str ? str : "";
}

const char* i18nGetLanguageName(Language lang) {
    if (lang < 0 || lang >= LANG_COUNT) {
        return "";
    }
    switch (lang) {
        case LANG_ENGLISH:
            return strings[LANG_ENGLISH][STR_ENGLISH];
        case LANG_CHINESE_SIMPLIFIED:
            return strings[LANG_CHINESE_SIMPLIFIED][STR_CHINESE_SIMPLIFIED];
        case LANG_CHINESE_TRADITIONAL:
            return strings[LANG_CHINESE_TRADITIONAL][STR_CHINESE_TRADITIONAL];
        case LANG_JAPANESE:
            return strings[LANG_JAPANESE][STR_JAPANESE];
        default:
            return "";
    }
}
