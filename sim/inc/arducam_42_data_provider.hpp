#ifndef NOS3_ARDUCAM42DATAPROVIDER_HPP
#define NOS3_ARDUCAM42DATAPROVIDER_HPP

#include <boost/property_tree/ptree.hpp>

#include <sim_data_42socket_provider.hpp>

namespace Nos3
{
    // vvv This is pretty standard for a 42 data provider (if one is needed for your sim)
    class Arducam42DataProvider : public SimData42SocketProvider
    {
    public:
        // Constructors / destructor
        Arducam42DataProvider(const boost::property_tree::ptree& config);

        // Accessors
        boost::shared_ptr<SimIDataPoint> get_data_point(void) const;

    private:
        // Disallow these
        ~Arducam42DataProvider(void) {};
        Arducam42DataProvider& operator=(const Arducam42DataProvider&) {};

        int16_t _sc;  // Which spacecraft number to parse out of 42 data
    };
}

#endif
