使用该 esp32 透传模块时，先使用 TX0 与 RX0 将 micropython 文件烧录
烧录完成再次上电后 esp32 建立 wifi_ap，wifi 名称与密码可以自定义，默认 essid="esp32", password="12345678"
连接 wifi 后在端口 32 建立 tcp 连接
建立 tcp 连接后，wifi 与 uart2 的收发建立透传
