BLE_GAP_AD_TYPE_FLAGS       = 0x01
BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE       = 0x02
BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE     = 0x03
BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE       = 0x04
BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE     = 0x05
BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE      = 0x06
BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE    = 0x07
BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME    = 0x08
BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME     = 0x09
BLE_GAP_AD_TYPE_TX_POWER_LEVEL      = 0x0A
BLE_GAP_AD_TYPE_CLASS_OF_DEVICE     = 0x0D
BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C       = 0x0E
BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R     = 0x0F
BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE       = 0x10
BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS      = 0x11
BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     = 0x12
BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       = 0x14
BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      = 0x15
BLE_GAP_AD_TYPE_SERVICE_DATA    = 0x16
BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS       = 0x17
BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS       = 0x18
BLE_GAP_AD_TYPE_APPEARANCE      = 0x19
BLE_GAP_AD_TYPE_ADVERTISING_INTERVAL    = 0x1A
BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS     = 0x1B
BLE_GAP_AD_TYPE_LE_ROLE     = 0x1C
BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C256    = 0x1D
BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256      = 0x1E
BLE_GAP_AD_TYPE_SERVICE_DATA_32BIT_UUID     = 0x20
BLE_GAP_AD_TYPE_SERVICE_DATA_128BIT_UUID    = 0x21
BLE_GAP_AD_TYPE_URI     = 0x24
BLE_GAP_AD_TYPE_3D_INFORMATION_DATA     = 0x3D
BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA      = 0xFF

import struct

def get_adv_section(adv_data, section):
    i = 0
    while i < len(adv_data):
        if (i+2) > len(adv_data):
            return None
        seg_len = adv_data[i]
        seg_type = adv_data[i+1]
        if seg_type == section:
            return adv_data[i+2:i+2+seg_len]
        i += seg_len + 1
    return None


class PduAdvIndirected:
    def __init__(self, _data):
        self.data = _data
        self.address = self.data[:6]
        self.adv_data = self.data[6:]
        self.rssi = 0
        self.name = ""

        adv_name_seg = get_adv_section(self.adv_data, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME)
        if adv_name_seg is not None:
            self.name = str(bytes(adv_name_seg), encoding='utf8')

    def addr_hex(self):
        return ":".join('%02x'%x for x in self.address[::-1])

    def __repr__(self):
        return "<adv ind> ({}) {} {}".format( self.addr_hex(), self.rssi, self.name)

