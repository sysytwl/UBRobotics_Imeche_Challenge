#ifndef DRV8323S_H
#define DRV8323S_H

#include <SPI.h>
#include <Arduino.h>

#define Fault_Status_1 0x0
#define VGS_Status_2 0x1
#define Driver_Control 0x2
#define Gate_Drive_HS 0x3
#define Gate_Drive_LS 0x4
#define OCP_Control 0x5
#define CSA_Control 0x6
//#define Reserved 0x7
#define Reserved_bit 0b0

/*
R/W ADDRESS         DATA
B15 B14 B13 B12 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0       For a write command (W0 = 0), For a read command (W0 = 1)
W0  A3  A2  A1  A0  D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
*/

class DRV8323S {
 public:
    DRV8323S(int8_t SCK, int8_t MISO, int8_t MOSI, int8_t SS);

    struct FaultStatus {
        uint8_t vds_lc;
        uint8_t vds_hc;
        uint8_t vds_lb;
        uint8_t vds_hb;
        uint8_t vds_la;
        uint8_t vds_ha;
        uint8_t otsd;
        uint8_t uvlo;
        uint8_t gdf;
        uint8_t vds_ocp;
        uint8_t fault;
    } FaultStatus_;

    struct VGSStatus {
        uint8_t vgs_lc;
        uint8_t vgs_hc;
        uint8_t vgs_lb;
        uint8_t vgs_hb;
        uint8_t vgs_la;
        uint8_t vgs_ha;
        uint8_t cpuv;
        uint8_t otw;
        uint8_t sc_oc;
        uint8_t sb_oc;
        uint8_t sa_oc;
    } VGSStatus_;

    struct DriverControl {
        uint8_t CLR_FLT;
        uint8_t BRAKE;
        uint8_t COAST;
        uint8_t PWM_MODE0;
        uint8_t PWM_MODE1;
        uint8_t OTW_REP;
        uint8_t DIS_GDF;
        uint8_t DIS_CPUV;
        uint8_t PWM_DIR;
        uint8_t PWM_COM;
    } DriverControl_;

    bool SetDriverControl();
    bool SetGateDriveHS();
    bool SetGateDriveLS();
    bool SetOCPControl();
    bool SetCSAControl();

    void get_status();

private:
    int8_t _SCK, _MISO, _MOSI, _SS;
    static const int _spiClk = 1000; // 1kHz

    bool write_reg(uint16_t data);
    uint16_t read_reg(uint8_t address);
};

#endif // DRV8323S_H
