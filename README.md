# Example for connection with Alibaba Cloud IoT Platform on Nuvoton's Mbed Enabled boards

This is an example to show connection with [Alibaba Cloud IoT Platform](https://iot.console.aliyun.com) on Nuvoton's Mbed Enabled boards.
Besides [Mbed OS](https://github.com/ARMmbed/mbed-os), it relies on the following modules:

-   [Alibaba Cloud IoT C-SDK port](https://github.com/OpenNuvoton/NuMaker-mbed-Aliyun-IoT-CSDK):
    Port of Alibaba Cloud IoT C-SDK onto Mbed OS, especially on Nuvoton's Mbed Enabled boards.

## Support targets

Platform                        |  Connectivity     | Storage for credentials and FW candidate  | Notes
--------------------------------|-------------------|-------------------------------------------|---------------
Nuvoton NUMAKER_PFM_NUC472      | Ethernet          | NU SD card                                |
Nuvoton NUMAKER_PFM_M487        | Ethernet          | NU SD card                                |
Nuvoton NUMAKER_IOT_M487        | Wi-Fi ESP8266     | NU SD card                                |
Nuvoton NUMAKER_IOT_M263A       | Wi-Fi ESP8266     | NU SD card                                |

## Support development tools

-   [Arm's Mbed Online Compiler](https://os.mbed.com/docs/mbed-os/v5.15/tools/developing-mbed-online-compiler.html)
-   [Arm's Mbed Studio](https://os.mbed.com/docs/mbed-os/v5.15/tools/developing-mbed-studio.html)
-   [Arm's Mbed CLI](https://os.mbed.com/docs/mbed-os/v5.15/tools/developing-mbed-cli.html)

## Developer guide

This section is intended for developers to get started, import the example application, compile with Mbed CLI, and get it running and connecting with Alibaba Cloud IoT Platform.

### Hardware requirements

-   Nuvoton's Mbed Enabled board
-   Micro SD card

### Software requirements

-   [Arm's Mbed CLI](https://os.mbed.com/docs/mbed-os/v5.15/tools/developing-mbed-cli.html)
-   Alibaba Cloud account

### Hardware setup

1.  Insert micro SD card into target board
1.  Connect target board to host through USB

### Operations on IoT Platform console

1.  Lon on to [IoT Platform console](http://iot.console.aliyun.com).
1.  Create a product/device. Take note of the acquired trituple: **ProductKey**/**DeviceName**/**DeviceSecret**. These are required to change to example code in the below section.
    1.  [Create a product](https://github.com/AlibabaCloudDocs/iot/blob/master/intl.en-US/User%20Guide/Create%20products%20and%20devices/Create%20a%20product.md)
    1.  [Create a device](https://github.com/AlibabaCloudDocs/iot/blob/master/intl.en-US/User%20Guide/Create%20products%20and%20devices/Create%20devices/Create%20a%20device.md)
1.  After device connects, interact with it. For example, publish a message *hello aliyun iot* to the topic `/${ProductKey}>/${DeviceName}>/user/get`:
    1.  In the left-side navigation pane, click **Devices** > **Devices**.
    1.  In the **Add Device** tab, choose target device `${DeviceName}`.
    1.  In the `${DeviceName}` page, click **Topic List** tab.
    1.  In the **Topic List** tab, click **publish** in the `/${ProductKey}/${DeviceName}/user/get` row.
    1.  In the **Publish** page, enter *hello aliyun iot* in the **Notification Content** field. And you should see the *hello aliyun iot* message in the host console in the below section.

### Compile with Mbed CLI

In the following, we take [NuMaker-IoT-M487](https://os.mbed.com/platforms/NUMAKER-IOT-M487/) as example board to show this example.

1.  Clone the example and navigate into it
    ```sh
    $ git clone https://github.com/OpenNuvoton/NuMaker-mbed-Aliyun-IoT-CSDK-example
    $ cd NuMaker-mbed-Aliyun-IoT-CSDK-example
    ```
1.  Deploy necessary libraries
    ```sh
    $ mbed deploy
    ```
1.  Configure network interface
    -   Ethernet: In `mbed_app.json`, just note resource allocation for lwIP. Might need modifications for some applications.
        ```json
            "lwip.pbuf-pool-size"                       : 10,
            "lwip.mem-size"                             : 3200,
        ```
    -   WiFi: In `mbed_app.json`, configure WiFi **SSID**/**PASSWORD**.
        ```json
            "nsapi.default-wifi-ssid"                   : "\"SSID\"",
            "nsapi.default-wifi-password"               : "\"PASSWORD\"",
        ```
1.  In `source/mqtt_example.c`, change **ProductKey**/**DeviceName**/**DeviceSecret** to acquired above. **ProductSecret** doesn't matter here.
    ```C
    char g_product_key[IOTX_PRODUCT_KEY_LEN + 1]       = "FIXME";
    char g_product_secret[IOTX_PRODUCT_SECRET_LEN + 1] = "IGNORED";
    char g_device_name[IOTX_DEVICE_NAME_LEN + 1]       = "FIXME";
    char g_device_secret[IOTX_DEVICE_SECRET_LEN + 1]   = "FIXME";
    ```
1.  Build the example on **NUMAKER_IOT_M487** target and **ARMC6** toolchain
    ```sh
    $ mbed compile -m NUMAKER_IOT_M487 -t ARMC6
    ```
1.  Flash by drag-n-drop'ing the built image file below onto **NuMaker-IoT-M487** board

    `BUILD/NUMAKER_IOT_M487/ARMC6/NuMaker-mbed-Aliyun-IoT-example.bin`

    **Note**: By drag-n-drop flash, the device is reset for clean start, including kvstore reset and so user filesystem re-initialization.

### Monitor the application through host console

Configure host terminal program with **115200/8-N-1**, and you should see log similar to below:

```
The device has not provisioned yet. Try to provision it...
Provision for development...
Reset kvstore...
Reset kvstore...OK
Inject ROT key...
Inject ROT key...OK
Mount user filesystem...
User filesystem region: start/end address: 0000000004400000/0000000076100000
Mount user filesystem...OK
Provision for development...OK
main|158 :: mqtt example
[wrn] IOT_MQTT_Construct(274): Using default hostname: 'a1wOVhf0PmQ.iot-as-mqtt.cn-shanghai.aliyuncs.com'
[wrn] IOT_MQTT_Construct(281): Using default port: [443]
[wrn] IOT_MQTT_Construct(288): Using default client_id: a1wOVhf0PmQ.T10YogSxts4YVtugH5at|timestamp=2524608000000,_v=sdk-c-3.1.0,securemode=2,signmethod=hmacsha256,lan=C,_ss=1,gw=0,ext=0|
[wrn] IOT_MQTT_Construct(295): Using default username: T10YogSxts4YVtugH5at&a1wOVhf0PmQ
[wrn] IOT_MQTT_Construct(303): Using default password: ******
[wrn] IOT_MQTT_Construct(316): Using default request_timeout_ms: 5000, configured value(0) out of [1000, 10000]
[wrn] IOT_MQTT_Construct(331): Using default keepalive_interval_ms: 60000, configured value(0) out of [30000, 1200000]
[wrn] IOT_MQTT_Construct(337): Using default read_buf_size: 3072
[wrn] IOT_MQTT_Construct(343): Using default write_buf_size: 3072
everything_state_handle|120 :: recv -0x0327(a1wOVhf0PmQ.iot-as-mqtt.cn-shanghai.aliyuncs.com)
everything_state_handle|120 :: recv -0x0327(443)
everything_state_handle|120 :: recv -0x0327(_v=sdk-c-3.1.0,securemode=2,signmethod=hmacsha256,lan=C,_ss=1,g)
everything_state_handle|120 :: recv -0x0327(T10YogSxts4YVtugH5at&a1wOVhf0PmQ)
everything_state_handle|120 :: recv -0x0327(588FC266DA82C9E3C68AB06BE447AA73AA4F41190EEA972C3808FF22D0DADD7)
[dbg] _mqtt_nwk_connect(2708): calling TCP or TLS connect HAL for [1/3] iteration
[err] HAL_Kv_Get(49): HAL_Kv_Get(seed_key) failed
Loading the CA root certificate ...
 ok (0 skipped)
start prepare client cert .
start mbedtls_pk_parse_key[]
Connecting to /a1wOVhf0PmQ.iot-as-mqtt.cn-shanghai.aliyuncs.com/443...
Connected to the network successfully. IP address: 192.168.8.109
 ok
  . Setting up the SSL/TLS structure...
 ok
Performing the SSL/TLS handshake...
 ok
  . Verifying peer X.509 certificate..
certificate verification result: 0x00
[dbg] _mqtt_nwk_connect(2726): rc = pClient->ipstack.connect() = 0, success @ [1/3] iteration
[inf] _mqtt_connect(722): connect params: MQTTVersion=4, clientID=a1wOVhf0PmQ.T10YogSxts4YVtugH5at|timestamp=2524608000000,_v=sdk-c-3.1.0,securemode=2,signmethod=hmacsha256,lan=C,_ss=1,gw=0,ext=0|, keepAliveInterval=120, username=T10YogSxts4YVtugH5at&a1wOVhf0PmQ
[inf] _mqtt_connect(768): mqtt connect success!
everything_state_handle|120 :: recv -0x0000(mqtt connected in 10472 ms)
[dbg] iotx_report_firmware_version(146): firmware version report start in MQTT
[dbg] iotx_report_firmware_version(159): firmware report topic: /ota/device/inform/a1wOVhf0PmQ/T10YogSxts4YVtugH5at
[dbg] iotx_report_firmware_version(172): firmware report data: {"id":"0","params":{"version":"00000000"}}
[inf] MQTTPublish(2588): Upstream Topic: '/ota/device/inform/a1wOVhf0PmQ/T10YogSxts4YVtugH5at'
[inf] MQTTPublish(2589): Upstream Payload:

> {
>     "id": "0",
>     "params": {
>         "version": "00000000"
>     }
> }

everything_state_handle|120 :: recv -0x0329(pub - '/ota/device/inform/a1wOVhf0PmQ/T10YogSxts4YVtugH5at': 1)
[dbg] iotx_report_firmware_version(181): firmware version report finished, iotx_publish() = 1
everything_state_handle|120 :: recv -0x0328(sub - '/a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get')
[dbg] wrapper_mqtt_subscribe(2917): PERFORM subscribe to '/a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get' (msgId=2)
[dbg] MQTTSubscribe(2145):         Packet Ident : 00000002
[dbg] MQTTSubscribe(2146):                Topic : /a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get
[dbg] MQTTSubscribe(2147):                  QoS : 0
[dbg] MQTTSubscribe(2148):        Packet Length : 49
[inf] wrapper_mqtt_subscribe(2928): mqtt subscribe packet sent,topic = /a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get!
[dbg] iotx_mc_cycle(1547): PUBACK
example_event_handle|093 :: msg->event_type : 9
[dbg] iotx_mc_cycle(1557): SUBACK
[dbg] iotx_mc_handle_recv_SUBACK(1055):         Return Value : 1
[dbg] iotx_mc_handle_recv_SUBACK(1056):            Packet ID : 2
[dbg] iotx_mc_handle_recv_SUBACK(1057):                Count : 1
[dbg] iotx_mc_handle_recv_SUBACK(1059):      Granted QoS[00] : 1
[dbg] _iotx_mqtt_event_handle_sub(1015): packet_id = 2, event_type=3
example_event_handle|093 :: msg->event_type : 3
[dbg] MQTTKeepalive(1659): len = MQTTSerialize_pingreq() = 2
[inf] iotx_mc_keepalive_sub(1712): send MQTT ping...
[inf] iotx_mc_cycle(1584): receive ping response!
```

From IoT Platform console, publish the message *hello aliyun iot* to the topic `/${ProductKey}/${DeviceName}/user/get` as done in the above section, and you should see:

<pre>
[dbg] iotx_mc_cycle(1565): PUBLISH
[inf] iotx_mc_handle_recv_PUBLISH(1402): Downstream Topic: '/a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get'
[inf] iotx_mc_handle_recv_PUBLISH(1403): Downstream Payload:
[dbg] iotx_mc_handle_recv_PUBLISH(1408):         Packet Ident : 00000000
[dbg] iotx_mc_handle_recv_PUBLISH(1409):         Topic Length : 42
[dbg] iotx_mc_handle_recv_PUBLISH(1413):           Topic Name : /a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get
[dbg] iotx_mc_handle_recv_PUBLISH(1416):     Payload Len/Room : 16 / 24
[dbg] iotx_mc_handle_recv_PUBLISH(1417):       Receive Buflen : 70
[dbg] iotx_mc_handle_recv_PUBLISH(1433): delivering msg ...
[dbg] iotx_mc_deliver_message(1253): topic be matched
example_message_arrive|026 :: Message Arrived:
example_message_arrive|027 :: Topic  : /a1wOVhf0PmQ/T10YogSxts4YVtugH5at/user/get
example_message_arrive|028 :: Payload: <b>hello aliyun iot</b>
example_message_arrive|029 ::
</pre>

### Walk through source code

#### Pre-main (`pre-main/`)

In Mbed OS boot sequence, `mbed_main()`, designed for user application override, is run before `main()`.
Here, it is used to run the following tasks:

1.  Simulate provision process for development
    1.  Reset [kvstore](https://os.mbed.com/docs/mbed-os/v5.15/reference/storage.html)
    1.  Inject entropy seed (if no entropy source) for mbedtls communication
    1.  Inject ROT key (device key) for firmware OTA
    1.  Initialize user filesystem (if enabled)
    1.  Mark the device as provisioned
    ```
    The device has not provisioned yet. Try to provision it...
    Provision for development...
    Reset kvstore...
    Reset kvstore...OK
    Inject ROT key...
    Inject ROT key...OK
    Mount user filesystem...
    User filesystem region: start/end address: 0000000004000000/0000000076100000
    Mount user filesystem...OK
    Provision for development...OK
    ```

1.  Set up event queue for dispatching host command. Currently, press the following command:
    1.  `h` for printing heap statistics
        ```
        ** MBED HEAP STATS **
        **** current_size: 28779
        **** max_size    : 32180
        *****************************
        ```

    1.  `s` for printing stack statistics
        ```
        ** MBED THREAD STACK STATS **
        Thread: 0x20003170, Stack size: 2048, Max stack: 552
        Thread: 0x20005da8, Stack size: 512, Max stack: 104
        Thread: 0x20005d64, Stack size: 8192, Max stack: 3440
        Thread: 0x20005dec, Stack size: 768, Max stack: 96
        *****************************
        ```

    1.  `r` for resetting system
        ```
        System reset after 2 secs...
        ```

#### Main with connection over MQTT (`source/mqtt_example.c`)

The examplle here is extract from [Alibaba Cloud IoT C-SDK](https://github.com/aliyun/iotkit-embedded) and shows connection with Alibaba Cloud IoT Platform over MQTT protocol.
Related explanation can be found in [Communication through MQTT Topics](https://www.alibabacloud.com/help/doc-detail/97568.htm).

#### Experimental examples (`TARGET_IGNORE/examples/`)

The examples here are experimental. They are not well tested and don't rely on them.

### Flash and storage layout

This section shows layout of flash and storage.

**Note**: For simplicity, gap and alignment requirements, e.g. flash erase size and storage block size, are not represented here.

#### Internal flash layout

    +---------------------------+ <-+ MBED_ROM_START + MBED_ROM_SIZE
    |        FREE SPACE         |
    |---------------------------| <-+ storage_filesystem.internal_base_address +
    |                           |     storage_filesystem.rbp_internal_size
    |    KVSTORE (internal)     |
    |                           |
    +---------------------------+ <-+ storage_filesystem.internal_base_address =
    |                           |     FLASHIAP_APP_ROM_END_ADDR
    |                           |
    |        ACTIVE APP         |
    |                           |
    |                           |
    +---------------------------+ <-+ MBED_ROM_START

-   `ACTIVE APP`: Area for placing application code. Its start is configurable and its size is determined by image file size.
-   `KVSTORE (internal)`: Area for kvstore internal part (located in internal flash)

**Note**: The symbols `MBED_ROM_START`/`MBED_ROM_SIZE` are generated by Mbed OS build tool. They mean start/size of internal flash.

**Note**: The symbol `FLASHIAP_APP_ROM_END_ADDR` comes from `mbed-os/drivers/FlashIAP.h`. It means end of application code.

**Note**: When `storage_filesystem.internal_base_address` equals 0, it means `FLASHIAP_APP_ROM_END_ADDR`.

#### External storage (`NUSD`) layout

    +---------------------------+ <-+ user-filesystem.blockdevice-address +
    |                           |     user-filesystem.blockdevice-size
    |     USER FILESYSTEM       |
    |                           |
    |---------------------------| <-+ user-filesystem.blockdevice-address
    |           GAP             |
    +---------------------------+ <-+ storage_filesystem.external_base_address +
    |                           |     storage_filesystem.external_size
    |    KVSTORE (external)     |
    |                           |
    +---------------------------+ <-+ storage_filesystem.external_base_address

-   `KVSTORE (external)`: Area for kvstore external part (located in external storage)
-   `USER FILESYSTEM`: Area for user filesystem. It is defined in `user-filesystem/mbed_lib.json`.

**Note**: When `user-filesystem.blockdevice-size` equals 0, it means extension to end of storage.

**Note**: User must guarantee these areas don't overlap.
