#ifndef _WIRELESS_GATEWAY_TEST_CASE_HPP_
#define _WIRELESS_GATEWAY_TEST_CASE_HPP_

class WirelessGatewayTestCase {
public:
    static void runAllTests();

private:
    static void testHTTPServerSetup();
    static void testKICEndpointResponse();
    static void testStatusEndpointResponse();
};

#endif // _WIRELESS_GATEWAY_TEST_CASE_HPP_

