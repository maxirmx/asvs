#pragma once

// This class is used to distinguish between 400 and 500 response codes
class AsVsException : public std::exception
{
public:
    explicit AsVsException(const std::string& message) : std::exception(), msg(message) {  }
    virtual ~AsVsException() noexcept {}
    virtual const char* what() const noexcept { return msg.c_str(); }
protected:
    std::string msg;
};


class InternalError : public std::exception
{
public:
    explicit InternalError(const std::string & message) : std::exception(), msg(message) {  }
    virtual ~InternalError() noexcept {}
    virtual const char* what() const noexcept { return msg.c_str(); }
protected:
    std::string msg;
};

inline std::chrono::system_clock::time_point str2time(const std::string& s)
{
    using namespace boost::posix_time;
    using namespace std::chrono;

    const ptime ts = time_from_string(s);
    auto seconds = to_time_t(ts);
    time_duration td = ts.time_of_day();
    auto microseconds = td.fractional_seconds();
    auto d = std::chrono::seconds{ seconds } + std::chrono::microseconds{ microseconds };
    system_clock::time_point tp{ duration_cast<system_clock::duration>(d) };
    return tp;
}


inline static std::string time2str(std::chrono::system_clock::time_point point)
{
    const std::time_t time = std::chrono::system_clock::to_time_t(point);
    auto info = localtime(&time);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info);
    return std::string(buffer);
}



