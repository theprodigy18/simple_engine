#include "utilities/leak_tracker.hpp"
#include "utilities/logger.hpp"

#include <mutex>
#include <vector>

//#ifdef _DEBUG
namespace
{
    struct Entry
    {
        void*       ptr;
        LeakType    type;
        const char* file;
        int         line;
        const char* desc;
        bool        freed {false};
    };

    std::vector<Entry> entries;
    std::mutex         mutex;

    const char* ToString(LeakType type)
    {
        switch (type)
        {
        case LeakType::HEAP:
            return "HEAP";
        case LeakType::OPENGL:
            return "OPENGL";
        case LeakType::HANDLE:
            return "HANDLE";
        case LeakType::CUSTOM:
            return "CUSTOM";
        default:
            return "UNKNOWN";
        }
    }

} // namespace anonymous

void _Register(void* ptr, LeakType type, const char* file, int line, const char* desc)
{
    std::lock_guard<std::mutex> lock(mutex);
    entries.push_back({ptr, type, file, line, desc, false});
}

void _Unregister(void* ptr)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& e : entries)
    {
        if (e.ptr == ptr && !e.freed)
        {
            e.freed = true;
            return;
        }
    }
    SM_LEAK_INFO("Failed to unregister pointer: %p, it was not found or has already been freed.", ptr);
}

void _ReportLeaks()
{
    std::lock_guard<std::mutex> lock(mutex);

    bool leakFound {false};
    for (const auto& e : entries)
    {
        if (!e.freed)
        {
            leakFound = true;
            SM_LEAK_INFO("Memory leak detected at pointer: %p, type: %s, file: %s, line: %d, desc: %s", e.ptr, ToString(e.type), e.file, e.line, e.desc);
        }
    }

    if (!leakFound)
    {
        SM_LEAK_INFO("No memory leaks detected");
    }
}

//#endif // _DEBUG
