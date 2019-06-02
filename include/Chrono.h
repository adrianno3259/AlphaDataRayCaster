#ifndef _CHRONO_H_
#define _CHRONO_H_

#include <chrono>
#include <ostream>

class Chrono
{
    
    std::ostream& m_stream;

    std::chrono::high_resolution_clock::time_point m_initTime;

public:

    Chrono(std::ostream& os) : 
        m_stream(os) {}

    ~Chrono(){}

    void start()
    {
        using namespace std::chrono;
        m_initTime = high_resolution_clock::now();
    }

    void stop(const std::string& description)
    {
        using namespace std::chrono;

        auto presentTime = high_resolution_clock::now();
        
        auto elapsedTime =
            duration_cast<duration<double>>(
                presentTime - m_initTime);

        m_stream 
            << description << ": " 
            << elapsedTime.count() << " seconds\n";
    }
};


#endif
