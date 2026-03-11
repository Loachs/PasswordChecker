#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstring>

using namespace std;

string toLowerStr(string_view s) {
    string out(s);
    for (char& c : out) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

bool allDigits(string_view s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

string normalizeLeetspeak(string_view s) {
    string out;
    out.reserve(s.size());

    for (char c : s) {
        char lower = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        switch (lower) {
            case '@': case '4': out += 'a'; break;
            case '3': out += 'e'; break;
            case '1': case '!': out += 'l'; break;
            case '0': out += 'o'; break;
            case '$': case '5': out += 's'; break;
            case '7': out += 't'; break;
            case '8': out += 'b'; break;
            default: out += lower; break;
        }
    }

    return out;
}

int countPassphraseWords(string_view s) {
    int wordCount = 0;
    bool inWord = false;

    for (char c : s) {
        if (c == ' ' || c == '-' || c == '_' || c == '.') {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            wordCount++;
        }
    }

    return wordCount;
}

bool isPassphrase(string_view s) {
    return countPassphraseWords(s) >= 4 && s.length() >= 16;
}

double calculatePassphraseEntropyRaw(int wordCount) {
    const double dictionarySize = 7776.0;
    return wordCount * log2(dictionarySize);
}

struct Point {
    int x, y;
};

Point getQWERTYCoord(char c) {
    const string_view rows[4] = {
        "1234567890-=",
        "qwertyuiop[]",
        "asdfghjkl;'",
        "zxcvbnm,./"
    };

    char lowerC = static_cast<char>(tolower(static_cast<unsigned char>(c)));

    for (int y = 0; y < 4; ++y) {
        size_t x = rows[y].find(lowerC);
        if (x != string_view::npos) {
            return { static_cast<int>(x), y };
        }
    }

    return { -1, -1 };
}

bool hasSpatialKeyboardPattern(string_view s) {
    if (s.size() < 4) return false;

    int adjacentStreak = 1;

    for (size_t i = 1; i < s.size(); ++i) {
        Point p1 = getQWERTYCoord(s[i - 1]);
        Point p2 = getQWERTYCoord(s[i]);

        if (p1.x == -1 || p2.x == -1) {
            adjacentStreak = 1;
            continue;
        }

        int dx = abs(p1.x - p2.x);
        int dy = abs(p1.y - p2.y);

        if (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0)) {
            adjacentStreak++;
            if (adjacentStreak >= 4) return true;
        } else {
            adjacentStreak = 1;
        }
    }

    return false;
}

bool containsKeyboardWalk(string_view s) {
    string lower = toLowerStr(s);

    vector<string> walks = {
        "qwertyuiop", "asdfghjkl", "zxcvbnm",
        "poiuytrewq", "lkjhgfdsa", "mnbvcxz",
        "1234567890", "0987654321"
    };

    for (const auto& walk : walks) {
        for (size_t len = 4; len <= walk.size(); ++len) {
            for (size_t i = 0; i + len <= walk.size(); ++i) {
                string sub = walk.substr(i, len);
                if (lower.find(sub) != string::npos) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool hasSequentialSubstring(string_view s, int minRun = 4) {
    if (s.size() < static_cast<size_t>(minRun)) return false;

    int ascRun = 1;
    int descRun = 1;

    for (size_t i = 1; i < s.size(); ++i) {
        char prev = static_cast<char>(tolower(static_cast<unsigned char>(s[i - 1])));
        char curr = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));

        if (static_cast<int>(curr) == static_cast<int>(prev) + 1) ascRun++;
        else ascRun = 1;

        if (static_cast<int>(curr) == static_cast<int>(prev) - 1) descRun++;
        else descRun = 1;

        if (ascRun >= minRun || descRun >= minRun) return true;
    }

    return false;
}

bool hasRepeatedCharacters(string_view s) {
    if (s.size() < 3) return false;

    int streak = 1;
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            streak++;
            if (streak >= 3) return true;
        } else {
            streak = 1;
        }
    }

    return false;
}

bool hasRepeatedPattern(string_view s) {
    if (s.size() < 6) return false;

    for (size_t len = 1; len <= s.size() / 2; ++len) {
        for (size_t start = 0; start + 2 * len <= s.size(); ++start) {
            string_view first = s.substr(start, len);
            string_view second = s.substr(start + len, len);
            if (first == second) return true;
        }
    }

    return false;
}

bool looksLikeBirthdate(string_view s) {
    if (!(s.size() == 6 || s.size() == 8)) return false;
    if (!allDigits(s)) return false;

    auto parseInt = [](string_view sv) -> int {
        int val = 0;
        for (char c : sv) val = val * 10 + (c - '0');
        return val;
    };

    if (s.size() == 8) {
        int a = parseInt(s.substr(0, 2));
        int b = parseInt(s.substr(2, 2));
        int year1 = parseInt(s.substr(4, 4));
        int year2 = parseInt(s.substr(0, 4));

        bool mmddyyyy = (a >= 1 && a <= 12 && b >= 1 && b <= 31 && year1 >= 1900 && year1 <= 2099);
        bool yyyymmdd = (year2 >= 1900 && year2 <= 2099);

        return mmddyyyy || yyyymmdd;
    }

    int a = parseInt(s.substr(0, 2));
    int b = parseInt(s.substr(2, 2));
    int c = parseInt(s.substr(4, 2));
    return (a >= 1 && a <= 12 && b >= 1 && b <= 31 && c >= 0 && c <= 99);
}

int estimateCharsetSize(string_view s) {
    int pool = 0;
    bool l = false, u = false, d = false, sym = false, space = false;

    for (char c : s) {
        if (islower(static_cast<unsigned char>(c))) l = true;
        else if (isupper(static_cast<unsigned char>(c))) u = true;
        else if (isdigit(static_cast<unsigned char>(c))) d = true;
        else if (c == ' ') space = true;
        else sym = true;
    }

    if (l) pool += 26;
    if (u) pool += 26;
    if (d) pool += 10;
    if (sym) pool += 33;
    if (space) pool += 1;

    return pool;
}

double calculateEffectiveLength(string_view s) {
    if (s.empty()) return 0.0;

    double effectiveLength = 1.0;
    int streak = 1;

    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            streak++;
            effectiveLength += (1.0 / streak);
        } else {
            streak = 1;
            effectiveLength += 1.0;
        }
    }

    return effectiveLength;
}

double calculateEntropyBits(string_view s) {
    int pool = estimateCharsetSize(s);
    if (pool == 0 || s.empty()) return 0.0;

    double effectiveLen = calculateEffectiveLength(s);
    return effectiveLen * log2(static_cast<double>(pool));
}

class BloomFilter {
private:
    vector<bool> bit_array;
    size_t size;

    uint64_t hash1(string_view s) const {
        uint64_t hash = 14695981039346656037ULL;
        for (char c : s) {
            hash ^= static_cast<uint8_t>(c);
            hash *= 1099511628211ULL;
        }
        return hash;
    }

    uint64_t hash2(string_view s) const {
        uint64_t hash = 5381;
        for (char c : s) {
            hash = ((hash << 5) + hash) + static_cast<uint8_t>(c);
        }
        return hash;
    }

public:
    explicit BloomFilter(size_t expected_elements) {
        size = max<size_t>(expected_elements * 10, 1000);
        bit_array.resize(size, false);
    }

    void add(string_view s) {
        bit_array[hash1(s) % size] = true;
        bit_array[hash2(s) % size] = true;
    }

    bool isLikelyBreached(string_view s) const {
        return bit_array[hash1(s) % size] && bit_array[hash2(s) % size];
    }
};

double calculateDictionaryPenalty(string_view s, const vector<string>& commonDict) {
    if (s.empty()) return 0.0;

    string lowerS = toLowerStr(s);

    for (const auto& word : commonDict) {
        if (word.length() < 3) continue;

        size_t pos = lowerS.find(word);
        while (pos != string::npos) {
            lowerS.replace(pos, word.length(), string(word.length(), '*'));
            pos = lowerS.find(word);
        }
    }

    int coveredChars = static_cast<int>(count(lowerS.begin(), lowerS.end(), '*'));
    return static_cast<double>(coveredChars) / static_cast<double>(s.length());
}

double calculatePassphraseEntropyAdjusted(string_view s, const vector<string>& dictionary) {
    int words = countPassphraseWords(s);
    if (words <= 0) return 0.0;

    double idealBits = calculatePassphraseEntropyRaw(words);
    double coverage = calculateDictionaryPenalty(s, dictionary);

    double adjustment = 1.0 - (coverage * 0.6);
    adjustment = clamp(adjustment, 0.4, 1.0);

    return idealBits * adjustment;
}

struct CrackEstimate {
    long double log10Guesses = 0.0L;
    long double log10Seconds = 0.0L;
    long double log10Years = 0.0L;
};

CrackEstimate estimateCrackTimeLogSpace(long double entropyBits, long double guessesPerSecond) {
    const long double LOG10_2 = 0.301029995663981195L;
    const long double SECONDS_PER_YEAR = 31557600.0L;

    long double log10Guesses = entropyBits * LOG10_2;
    long double log10Seconds = log10Guesses - log10(guessesPerSecond);
    long double log10Years = log10Seconds - log10(SECONDS_PER_YEAR);

    return { log10Guesses, log10Seconds, log10Years };
}

string formatScientific(long double log10Value) {
    if (!isfinite(static_cast<double>(log10Value))) return "overflow";

    int exponent = static_cast<int>(floor(log10Value));
    long double mantissa = pow(10.0L, log10Value - exponent);

    ostringstream out;
    out << fixed << setprecision(2) << mantissa << "e" << exponent;
    return out.str();
}

string formatTimeSmart(long double log10Seconds) {
    const long double SECONDS_PER_YEAR = 31557600.0L;

    if (!isfinite(static_cast<double>(log10Seconds))) return "overflow";
    if (log10Seconds < 0.0L) return "less than 1 second";

    if (log10Seconds < 15.0L) {
        long double seconds = pow(10.0L, log10Seconds);

        long long years = static_cast<long long>(seconds / 31557600.0L);
        seconds -= years * 31557600.0L;

        long long days = static_cast<long long>(seconds / 86400.0L);
        seconds -= days * 86400.0L;

        long long hours = static_cast<long long>(seconds / 3600.0L);
        seconds -= hours * 3600.0L;

        long long minutes = static_cast<long long>(seconds / 60.0L);
        seconds -= minutes * 60.0L;

        long long secs = static_cast<long long>(seconds);

        ostringstream out;
        if (years) out << years << " years ";
        if (days) out << days << " days ";
        if (hours) out << hours << " hours ";
        if (minutes) out << minutes << " minutes ";
        if (secs || (!years && !days && !hours && !minutes)) out << secs << " seconds";

        return out.str();
    }

    long double log10Years = log10Seconds - log10(SECONDS_PER_YEAR);
    return formatScientific(log10Years) + " years";
}

int calculateModernScore(double entropyBits, bool isBreached) {
    if (isBreached) return 0;

    const double LOG10_2 = 0.3010299956639812;
    double log10Guesses = entropyBits * LOG10_2;

    if (log10Guesses < 10.0) return 10;
    if (log10Guesses < 12.0) return 25;
    if (log10Guesses < 14.0) return 50;
    if (log10Guesses < 17.0) return 75;
    if (log10Guesses >= 20.0) return 100;

    int dynamicScore = static_cast<int>((log10Guesses - 10.0) * 10.0);
    return clamp(dynamicScore, 0, 100);
}

string strengthLabel(int score) {
    if (score == 0) return "COMPROMISED";
    if (score <= 10) return "Extremely Weak";
    if (score <= 20) return "Very Weak";
    if (score <= 30) return "Weak";
    if (score <= 40) return "Bad";
    if (score <= 50) return "Fair";
    if (score <= 60) return "Moderate";
    if (score <= 70) return "Good";
    if (score <= 80) return "Strong";
    if (score <= 90) return "Very Strong";
    return "Extremely Strong";
}

struct PasswordAnalysis {
    int score = 0;
    double entropyBits = 0.0;
    double dictCoverage = 0.0;
    bool isPassphrase = false;
    bool isBreached = false;
    bool repeatedChars = false;
    bool repeatedPattern = false;
    bool sequential = false;
    bool spatialKeyboard = false;
    bool birthdateLike = false;
    vector<string> feedback;
};

static BloomFilter buildBreachDB() {
    vector<string> dummyBreaches = {
        "password", "123456", "qwerty", "admin", "p@ssw0rd", "letmein123",
        "password1", "welcome", "dragon", "football", "monkey", "abc123"
    };

    BloomFilter breachDB(dummyBreaches.size());
    for (const auto& breach : dummyBreaches) {
        breachDB.add(toLowerStr(breach));
    }
    return breachDB;
}

static vector<string> buildDictionary() {
    return {
        "password", "admin", "login", "user", "pink", "sunshine", "qwerty", "abc", "love",
        "test", "guest", "root", "pass", "monkey", "dragon", "football", "soccer", "baseball",
        "basketball", "hockey", "master", "access", "secret", "hidden", "change", "security",
        "shadow", "hunter", "trust", "family", "lucky", "happy", "summer", "winter", "spring",
        "autumn", "charlie", "superman", "batman", "pokemon", "matrix", "server", "system",
        "oracle", "yellow", "purple", "ginger", "pepper", "cookie", "princess", "mountain",
        "ocean", "river", "forest", "desert", "starlight", "garden", "flower", "coffee",
        "whiskey", "pizza", "cheese", "bacon", "muffin", "laptop", "mobile", "camera", "guitar",
        "window", "bridge", "street", "planet", "rocket", "galaxy", "stark", "skywalker",
        "wizard", "warrior", "knight", "legend", "pioneer", "vision", "dream", "freedom",
        "justice", "victory", "online", "active", "direct", "simple", "bright", "strong",
        "global", "united", "action", "player", "friend", "people", "james", "john", "robert",
        "michael", "william", "david", "richard", "joseph", "thomas", "charles", "mary",
        "patricia", "jennifer", "linda", "elizabeth", "barbara", "susan", "jessica", "sarah",
        "karen", "smith", "jones", "brown", "miller", "wilson", "taylor", "walker", "green",
        "white", "black", "brave", "smart", "quick", "silent", "golden", "silver", "metal",
        "stone", "water", "earth", "fire", "wind", "storm", "cloud", "rain", "snow", "light",
        "dark", "night", "create", "delete", "update", "manage", "select", "insert", "follow",
        "lookup", "search", "review", "upload", "submit", "finish", "start", "ready", "please",
        "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "january",
        "february", "march", "april", "always", "forever", "never", "today", "office", "worker",
        "client", "remote", "tablet", "screen", "mouse", "click", "network", "router", "cloud",
        "backup", "restore", "format", "design", "sketch", "draft", "email", "inbox", "outbox",
        "attach", "folder", "london", "paris", "tokyo", "berlin", "moscow", "sydney", "austin",
        "boston", "europe", "america", "africa", "canada", "mexico", "brazil", "china", "india",
        "jupiter", "saturn", "venus", "mars", "pluto", "nebula", "comet", "meteor", "sugar",
        "butter", "bread", "honey", "lemon", "apple", "orange", "banana", "cherry", "grape",
        "melon", "peach", "berry", "tomato", "potato", "carrot", "onion", "garlic", "salt",
        "cat", "dog", "rabbit", "hamster", "turtle", "snake", "spider", "fish", "bird", "lion",
        "tiger", "bear", "wolf", "fox", "eagle", "shark", "dolphin", "whale", "octopus", "crab",
        "lobster", "ant", "bee", "butterfly", "dragonfly", "mosquito", "fly", "worm", "snail",
        "frog", "toad", "lizard", "crocodile", "pickle", "dad", "mom"
    };
}

PasswordAnalysis analyzePassword(
    const string& password,
    const BloomFilter& breachDB,
    const vector<string>& dictionary
) {
    PasswordAnalysis result;

    string lower = toLowerStr(password);
    string normalized = normalizeLeetspeak(password);

    result.isBreached = breachDB.isLikelyBreached(lower) || breachDB.isLikelyBreached(normalized);
    result.dictCoverage = calculateDictionaryPenalty(password, dictionary);
    result.isPassphrase = isPassphrase(password);

    if (result.isPassphrase) {
        result.entropyBits = calculatePassphraseEntropyAdjusted(password, dictionary);
    } else {
        result.entropyBits = calculateEntropyBits(password);
    }

    if (!result.isPassphrase && result.dictCoverage > 0.30) {
        result.entropyBits *= (1.0 - (result.dictCoverage * 0.7));
        result.feedback.push_back("Avoid mashing common dictionary words together unless it's a long passphrase.");
    }

    result.repeatedChars = hasRepeatedCharacters(password);
    result.repeatedPattern = hasRepeatedPattern(password);
    result.sequential = hasSequentialSubstring(password);
    result.spatialKeyboard = hasSpatialKeyboardPattern(password) || containsKeyboardWalk(password);
    result.birthdateLike = looksLikeBirthdate(password);

    if (!result.isPassphrase) {
        if (result.sequential) result.entropyBits *= 0.6;
        if (result.spatialKeyboard) result.entropyBits *= 0.6;
        if (result.repeatedPattern) result.entropyBits *= 0.7;
        if (result.birthdateLike) result.entropyBits = min(result.entropyBits, 20.0);
    }

    result.score = calculateModernScore(result.entropyBits, result.isBreached);

    if (result.isBreached) {
        result.feedback.push_back("CRITICAL: This password matches known breach patterns. Do not use it.");
    } else {
        if (password.length() < 12) {
            result.feedback.push_back("Length matters most. Make this at least 12-16 characters.");
        }

        if (result.isPassphrase) {
            result.feedback.push_back("Good use of a multi-word passphrase.");
            if (result.dictCoverage > 0.60) {
                result.feedback.push_back("This passphrase uses many common words, so it may be less random than it looks.");
            }
        } else {
            if (result.spatialKeyboard) {
                result.feedback.push_back("Avoid keyboard walks or adjacent-key patterns like qwerty / asdf / 1234.");
            }
            if (result.sequential) {
                result.feedback.push_back("Avoid sequential substrings like abcd or 1234.");
            }
            if (result.repeatedChars) {
                result.feedback.push_back("Avoid repeating the same character multiple times.");
            }
            if (result.repeatedPattern) {
                result.feedback.push_back("Avoid repeated chunks like abcabc or 121212.");
            }
            if (result.birthdateLike) {
                result.feedback.push_back("Avoid dates; they are easy to guess from public information.");
            }
            if (result.entropyBits < 60.0 && result.dictCoverage <= 0.30) {
                result.feedback.push_back("Make it longer and mix character types, or switch to a long random passphrase.");
            }
        }

        if (result.feedback.empty()) {
            result.feedback.push_back("Nice. This password avoids common weaknesses and has solid estimated entropy.");
        }
    }

    return result;
}

static string jsonEscape(const string& s) {
    string out;
    for (char c : s) {
        switch (c) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

extern "C" {
    const char* analyze_password_json(const char* input) {
        static string output;

        static BloomFilter breachDB = buildBreachDB();
        static vector<string> dictionary = buildDictionary();

        string password = input ? string(input) : "";
        PasswordAnalysis result = analyzePassword(password, breachDB, dictionary);

        auto online = estimateCrackTimeLogSpace(result.entropyBits, 1e3L);
        auto gpu = estimateCrackTimeLogSpace(result.entropyBits, 1e11L);

        ostringstream out;
        out << "{";
        out << "\"score\":" << result.score << ",";
        out << "\"strength\":\"" << jsonEscape(strengthLabel(result.score)) << "\",";
        out << "\"entropyBits\":" << fixed << setprecision(2) << result.entropyBits << ",";
        out << "\"dictCoverage\":" << fixed << setprecision(2) << (result.dictCoverage * 100.0) << ",";
        out << "\"isPassphrase\":" << (result.isPassphrase ? "true" : "false") << ",";
        out << "\"isBreached\":" << (result.isBreached ? "true" : "false") << ",";
        out << "\"sequential\":" << (result.sequential ? "true" : "false") << ",";
        out << "\"spatialKeyboard\":" << (result.spatialKeyboard ? "true" : "false") << ",";
        out << "\"repeatedChars\":" << (result.repeatedChars ? "true" : "false") << ",";
        out << "\"repeatedPattern\":" << (result.repeatedPattern ? "true" : "false") << ",";
        out << "\"birthdateLike\":" << (result.birthdateLike ? "true" : "false") << ",";
        out << "\"onlineTime\":\"" << jsonEscape(formatTimeSmart(online.log10Seconds)) << "\",";
        out << "\"gpuTime\":\"" << jsonEscape(formatTimeSmart(gpu.log10Seconds)) << "\",";
        out << "\"searchSpace\":\"" << jsonEscape(formatScientific(gpu.log10Guesses)) << "\",";
        out << "\"feedback\":[";
        for (size_t i = 0; i < result.feedback.size(); ++i) {
            if (i) out << ",";
            out << "\"" << jsonEscape(result.feedback[i]) << "\"";
        }
        out << "]";
        out << "}";

        output = out.str();
        return output.c_str();
    }
}
