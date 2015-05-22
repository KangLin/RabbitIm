文件传输
======

#### xmpp 文件传输协议：
1. [XEP-0096: SI File Transfer：文件传输流初始化协议](http://xmpp.org/extensions/xep-0096.html)
2. [XEP-0065: SOCKS5 Bytestreams：带外socks5代理字节流传输协议](http://xmpp.org/extensions/xep-0065.html)
3. [XEP-0047: In-Band Bytestreams:带内字节流传输协议](http://xmpp.org/extensions/xep-0047.html)
4. [XEP-0066: Out of Band Data:带外数据传输协议](http://xmpp.org/extensions/xep-0066.html)
5. [XEP-0030: Service Discovery:服务发现协议](http://xmpp.org/extensions/xep-0030.html)


* XEP-0096:文件传输流初始化协议。支持断点续传。当file元素包含range无素时，range包含offset、length两个属性，可以指出要传送的数据位于文件的什么位置。
* xep-0047:是把数据进行base64编码后，放到iq消息中转发。这个适用于小数据传输。  
* xep-0066：带外数据传输，是从发送端拉取。这个需要发送端位于公网。  
* xep-0065:带外数据传输，如果发送端位于公网，则接收端直接连接发送端拉取数据。如果双方都位于内网，则双方连接代理服务器，通过代理服务器转发数据。
* XEP-0030:服务发现协议，这里用来发现代理服务。

##### xep-0065 协议：


直接连接流程：


    Requester                         Target
       |                                |
       | Send S5B initiation request    |
       | -----------------------------> |
       |                                |
       | Open TCP socket                |
       | <_____________________________ |
       |                                |
       | Request SOCKS5 connection      |
       | <\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ |
       |                                |
       | Acknowledge SOCKS5 connection  |
       | /////////////////////////////> |
       |                                |
       | Send S5B acceptance            |
       | <----------------------------- |
       |                                |
       | Exchange data over S5B         |
       | <============================> |
       |                                |
       

间接连接流程：


    Requester                         Proxy                            Target
       |                                |                                |
       | Send S5B initiation request                                     |
       | --------------------------------------------------------------> |
       |                                |                                |
       |                                | Open TCP socket                |
       |                                | <_____________________________ |
       |                                |                                |
       |                                | Request SOCKS 5 connection     |
       |                                | <\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ |
       |                                |                                |
       |                                | Acknowledge SOCKS 5 connection |
       |                                | /////////////////////////////> |
       |                                |                                |
       | Send S5B acceptance                                             |
       | <-------------------------------------------------------------- |
       |                                |                                |
       | Open TCP socket                |                                |
       | _____________________________> |                                |
       |                                |                                |
       | Request SOCKS 5 connection     |                                |
       | /////////////////////////////> |                                |
       |                                |                                |
       | Acknowledge SOCKS 5 connection |                                |
       | <\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ |                                |
       |                                |                                |
       | Request activation             |                                |
       | -----------------------------> |                                |
       |                                |                                |
       | Acknowledge activation         |                                |
       | <----------------------------- |                                |
       |                                |                                |
       | Exchange data over S5B                                          |
       | <=============================================================> |
       |                                |                                |


#### qxmpp 文件传输：

qxmpp中的文件传输实现了：
* XEP-0096: SI File Transfer
* XEP-0047: In-Band Bytestreams
* XEP-0065: SOCKS5 Bytestreams
* XEP-0030: Service Discovery 

QXmppTransferManager：实现XEP-0096、XEP-0047、XEP-0065  
QXmppDiscoveryManager：实现 XEP-0030

##### 实例：
* 流初始化过程(XEP-0096)：  
发送端：


    <iq id="qxmpp73" to="c@rabbitim.com/QXmpp" type="set">
        <si xmlns="http://jabber.org/protocol/si" 
            id="MsVtTkTuwqKQhMBxUsuM66qLgEG9r4Cr" 
            profile="http://jabber.org/protocol/si/profile/file-transfer">
            <file xmlns="http://jabber.org/protocol/si/profile/file-transfer" 
                date="2009-07-14T04:52:25.650Z" 
                hash="076e3caed758a1c18c91a0e9cae3368f" 
                name="Chrysanthemum.jpg" 
                size="879394"/>
            <feature xmlns="http://jabber.org/protocol/feature-neg">
                <x xmlns="jabber:x:data" type="form">
                    <field type="list-single" var="stream-method">
                        <option><value>http://jabber.org/protocol/ibb</value></option>
                        <option><value>http://jabber.org/protocol/bytestreams</value></option>
                    </field>
                </x>
            </feature>
        </si>
    </iq>


目标端：


    <iq id="qxmpp73" to="b@rabbitim.com/QXmpp" type="result" from="c@rabbitim.com/QXmpp">
        <si xmlns="http://jabber.org/protocol/si" 
            profile="http://jabber.org/protocol/si/profile/file-transfer">
            <feature xmlns="http://jabber.org/protocol/feature-neg">
                <x xmlns="jabber:x:data" type="submit">
                    <field type="list-single" var="stream-method">
                        <value>http://jabber.org/protocol/bytestreams</value>
                    </field>
                </x>
            </feature>
        </si>
    </iq>


* 查询代理过程(XEP-0030):  
发送端：


    <iq id="qxmpp74" type="get"><query xmlns="http://jabber.org/protocol/disco#items" /></iq>


服务器回复信息，告知可用的代理：


    <iq type="result" id="qxmpp74" to="b@rabbitim.com/QXmpp">
        <query xmlns="http://jabber.org/protocol/disco#items">
            <item jid="proxy.rabbitim.com" name="Socks 5 Bytestreams Proxy"/>
            <item jid="pubsub.rabbitim.com" name="Publish-Subscribe service"/>
            <item jid="conference.rabbitim.com" name="公共房间"/>
            <item jid="search.rabbitim.com" name="User Search"/>
        </query>
    </iq>


这个代理查询过程，qxmpp有单独的类实现，查询到的结果用 QXmppTransferManager::setProxy 告知 QXmppTransferManager

* 得到代理配置过程：
这里选择name=“Socks 5 Bytestreams Proxy”的代理，初始方给这个代理发送信息获取代理连接信息  
发送端：


    <iq id="qxmpp75" to="proxy.rabbitim.com" type="get">
        <query xmlns="http://jabber.org/protocol/bytestreams" 
                sid="MsVtTkTuwqKQhMBxUsuM66qLgEG9r4Cr"/>
    </iq>


代理：


    <iq type="result" id="qxmpp75" from="proxy.rabbitim.com" to="b@rabbitim.com/QXmpp">
        <query xmlns="http://jabber.org/protocol/bytestreams">
            <streamhost jid="proxy.rabbitim.com" host="182.254.185.29" port="7777"/>
        </query>
    </iq>


* 连接代理过程（XEP-0065）：  
发送端：


    <iq id="qxmpp77" to="c@rabbitim.com/QXmpp" type="set">
        <query xmlns="http://jabber.org/protocol/bytestreams" 
               sid="MsVtTkTuwqKQhMBxUsuM66qLgEG9r4Cr">
               <streamhost host="192.168.1.101" jid="b@rabbitim.com/QXmpp" port="1346"/>
               <streamhost host="2001:0:5ef5:79fb:86b:f7e8:50fd:af01" jid="b@rabbitim.com/QXmpp" port="1346"/>
               <streamhost host="182.254.185.29" jid="proxy.rabbitim.com" port="7777"/>
        </query>
    </iq>

这里的 streamhost 是有顺序关系的。主机地址放在最前，代理放在最后。目标端会接这个顺序去连接。如果前面的可以连接上，就不在做后面的。
这个可以保证先直接连接，只有在直接连接不成功后，才连接代理，进行转发。

目标端：
目标端按上一步发过来的信息，按照streamhost的顺序连接。如果成功，则返回下面信息通知发送端。

    <iq id="qxmpp77" to="b@rabbitim.com/QXmpp" type="result" from="c@rabbitim.com/QXmpp">
        <query xmlns="http://jabber.org/protocol/bytestreams" 
            sid="MsVtTkTuwqKQhMBxUsuM66qLgEG9r4Cr" mode="tcp">
            <streamhost-used jid="proxy.rabbitim.com"/>
        </query>
    </iq>


* 发送端激活代理：
发送端：

    <iq id="qxmpp79" to="proxy.rabbitim.com" from="b@rabbitim.com/QXmpp" type="set">
        <query xmlns="http://jabber.org/protocol/bytestreams" sid="MsVtTkTuwqKQhMBxUsuM66qLgEG9r4Cr">
            <activate>c@rabbitim.com/QXmpp</activate>
        </query>
    </iq>


代理：


    <iq type="result" id="qxmpp79" from="proxy.rabbitim.com" to="b@rabbitim.com/QXmpp"/>


#### OpenFire 中的文件传输设置：

文件代理有下面几个设置：

* xmpp.filetransfer.enabled = true
* xmpp.proxy.service=proxy
* xmpp.proxy.enabled=ture
* xmpp.proxy.port=7777
* xmpp.proxy.externalip=127.0.0.1

上面这几个后面的值是默认值。前面四个用默认值就可以了，但最后一个需要设置为代理服务器的外网地址。
而代理服务器的jid：


        proxyServiceName = JiveGlobals.getProperty("xmpp.proxy.service", "proxy");
        /**
         * Returns the fully-qualifed domain name of this chat service.
         * The domain is composed by the service name and the
         * name of the XMPP server where the service is running.
         *
         * @return the file transfer server domain (service name + host name).
         */
        public String getServiceDomain() {
            return proxyServiceName + "." + XMPPServer.getInstance().getServerInfo().getXMPPDomain();
        }
        
        public JID getAddress() {
            return new JID(null, getServiceDomain(), null);
        }


所以，文件代理服务器的jid为下面两个配置值的组合：
xmpp.proxy.service、xmpp.domain

文件传输的代码位于源码目录：
src/java/org/jivesoftware/openfire/filetransfer

##### 总结：

* 配置域名：xmpp.domain为rabbitim.com
* 配置扩展IP：xmpp.proxy.externalip为文件代理服务器的外网ip
* 客户端程序查询到代理后，在发送文件前，要调用 QXmppTransferManager::setProxy 设置代理。
  本程序为：m_TransferManager.setProxy("proxy.rabbitim.com");
