<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>CreateChildNodeDialog</name>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="23"/>
        <source>Child Name:</source>
        <translation>子节点名称：</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="39"/>
        <source>Create Mode:</source>
        <translation>节点类型：</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="49"/>
        <source>PERSISTENT</source>
        <translation>持久节点</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="54"/>
        <source>EPHEMERAL</source>
        <translation>临时节点</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="59"/>
        <source>CONTAINER</source>
        <translation>容器</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="67"/>
        <source>sequence</source>
        <translation>序列节点</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="87"/>
        <source>Go after created</source>
        <translation>创建完成后跳转到该节点</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="97"/>
        <source>Close after created</source>
        <translation>创建完成后关闭该窗口</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="115"/>
        <source>Data:</source>
        <translation>节点值：</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="122"/>
        <source>has no data</source>
        <translation>空数据（NULL）</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="154"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.ui" line="161"/>
        <source>Create</source>
        <translation>创建</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.cpp" line="44"/>
        <source>Create Child For Node: </source>
        <translation>为该节点创建子节点：</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.cpp" line="55"/>
        <location filename="CreateChildNodeDialog.cpp" line="61"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.cpp" line="55"/>
        <source>Please enter the node name.</source>
        <translation>请输入子节点名。</translation>
    </message>
    <message>
        <location filename="CreateChildNodeDialog.cpp" line="61"/>
        <source>Node name must not contain &apos;/&apos;.</source>
        <translation>节点名称不能包含字符：&apos;/&apos;。</translation>
    </message>
</context>
<context>
    <name>DeleteNodeDialog</name>
    <message>
        <location filename="DeleteNodeDialog.ui" line="14"/>
        <source>Delete Node</source>
        <translation>删除节点</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.ui" line="20"/>
        <source>Path:</source>
        <translation>节点路径：</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.ui" line="41"/>
        <source>Recursively delete all children</source>
        <translation>递归删除子节点</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.ui" line="76"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.ui" line="83"/>
        <source>Delete</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.cpp" line="37"/>
        <source>Version Check</source>
        <translation>版本检查</translation>
    </message>
    <message>
        <location filename="DeleteNodeDialog.cpp" line="37"/>
        <source>current version is </source>
        <translation>当前版本是：</translation>
    </message>
</context>
<context>
    <name>EditConnectionsModel</name>
    <message>
        <location filename="EditConnectionsModel.h" line="36"/>
        <source>Connection Name</source>
        <translation>连接名称</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.h" line="36"/>
        <source>Hosts</source>
        <translation>地址（IP:Port）</translation>
    </message>
    <message>
        <source>Info</source>
        <translation type="vanished">信息</translation>
    </message>
    <message>
        <source>Can not find configuration file in Path: </source>
        <translation type="vanished">找不到配置文件：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="266"/>
        <location filename="EditConnectionsModel.cpp" line="275"/>
        <location filename="EditConnectionsModel.cpp" line="285"/>
        <location filename="EditConnectionsModel.cpp" line="290"/>
        <location filename="EditConnectionsModel.cpp" line="298"/>
        <location filename="EditConnectionsModel.cpp" line="303"/>
        <location filename="EditConnectionsModel.cpp" line="335"/>
        <source>Warn</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="266"/>
        <source>Failed to convert to QJsonDocument using data from file: </source>
        <translation>转换为Json文档失败：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="267"/>
        <source>Error: </source>
        <translation>错误：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="275"/>
        <source>Expect Object. File: </source>
        <translation>期望Json Object。文件：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="285"/>
        <source>Can not find field &apos;name&apos;. File: </source>
        <translation>找不到属性：name。文件：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="290"/>
        <location filename="EditConnectionsModel.cpp" line="303"/>
        <source>Expect String. File: </source>
        <translation>期望字符串。文件：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="298"/>
        <source>Can not find field &apos;hosts&apos;. File: </source>
        <translation>找不到属性：hosts。文件：</translation>
    </message>
    <message>
        <location filename="EditConnectionsModel.cpp" line="335"/>
        <source>Failed to open file: </source>
        <translation>打开文件失败：</translation>
    </message>
</context>
<context>
    <name>EditValueDialog</name>
    <message>
        <location filename="EditValueDialog.ui" line="14"/>
        <source>Edit Node Value</source>
        <translation>编辑节点值</translation>
    </message>
    <message>
        <location filename="EditValueDialog.ui" line="26"/>
        <source>Path:</source>
        <translation>节点路径：</translation>
    </message>
    <message>
        <location filename="EditValueDialog.ui" line="62"/>
        <source>Data:</source>
        <translation>节点值：</translation>
    </message>
    <message>
        <location filename="EditValueDialog.ui" line="87"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="EditValueDialog.ui" line="94"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="EditValueDialog.cpp" line="39"/>
        <source>Version Check </source>
        <translation>版本检查</translation>
    </message>
    <message>
        <location filename="EditValueDialog.cpp" line="39"/>
        <source>current version is </source>
        <translation>当前版本是：</translation>
    </message>
</context>
<context>
    <name>ZKClientGUI</name>
    <message>
        <location filename="ZKClientGUI.cpp" line="29"/>
        <source>Connect</source>
        <translation>连接</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.cpp" line="31"/>
        <source>Delete selected items</source>
        <translation>删除选中的连接信息</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.cpp" line="88"/>
        <source>Info</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.cpp" line="88"/>
        <source>Please edit connection detail and choose one connection firstly.</source>
        <translation>请先编辑一条连接信息并选中。</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.cpp" line="96"/>
        <source>Warn</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.cpp" line="96"/>
        <source>Failed to get zookeeper connection configuration.</source>
        <translation>获取zookeeper连接配置失败</translation>
    </message>
</context>
<context>
    <name>ZKClientGUIClass</name>
    <message>
        <location filename="ZKClientGUI.ui" line="14"/>
        <source>ZKClientGUI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="45"/>
        <source>Hosts</source>
        <translation>地址列表</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="61"/>
        <source>Edit Connections:</source>
        <translation>编辑连接信息：</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="123"/>
        <source>Connect</source>
        <translation>连接</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="163"/>
        <source>Need to restart this application.</source>
        <translation>需要重启应用</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="87"/>
        <source>Tips:</source>
        <translation>说明：</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="94"/>
        <source>1. Edit information of a zookeeper server.</source>
        <translation>1. 在上面的表格中填写ZooKeeper服务器的地址</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="101"/>
        <source>2. Press &apos;Connect&apos; button.</source>
        <translation>2. 按‘连接’按钮</translation>
    </message>
    <message>
        <location filename="ZKClientGUI.ui" line="137"/>
        <source>Language:</source>
        <translation>语言：</translation>
    </message>
    <message>
        <source>Need restart this application.</source>
        <translation type="vanished">需要重启应用</translation>
    </message>
</context>
<context>
    <name>ZKConnectionWorker</name>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="97"/>
        <source>the parent node does not exist.</source>
        <translation>父节点不存在。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="101"/>
        <source>the node does not exist.</source>
        <translation>节点不存在。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="104"/>
        <source>the client does not have permission.</source>
        <translation>客户端没有权限。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="106"/>
        <source>expected version does not match actual version.</source>
        <translation>期望的版本号与实际版本号不符。（已经有其他客户端进行其他操作了？）</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="108"/>
        <source>invalid input parameters</source>
        <translation>输入的参数异常。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="110"/>
        <source>zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE</source>
        <translation>连接Session过期了，或者身份验证失败了。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="112"/>
        <source>failed to marshall a request; possibly, out of memory</source>
        <translation>生成请求失败。也许内存不足了？</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="114"/>
        <source>the node already exists</source>
        <translation>节点已经存在。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="116"/>
        <source>cannot create children of ephemeral nodes.</source>
        <translation>不能为临时节点创建子节点。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="118"/>
        <source>children are present; node cannot be deleted.</source>
        <translation>这个节点还有子节点，因此不能删除。</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="120"/>
        <source>Unknown zk error: </source>
        <translation>未知的zk错误：</translation>
    </message>
    <message>
        <location filename="ZKConnectionWorker.cpp" line="166"/>
        <location filename="ZKConnectionWorker.cpp" line="198"/>
        <location filename="ZKConnectionWorker.cpp" line="238"/>
        <source>ZK is not connected.</source>
        <translation>没有连接到ZK服务器</translation>
    </message>
</context>
<context>
    <name>ZKDataViewWidget</name>
    <message>
        <location filename="ZKDataViewWidget.ui" line="14"/>
        <source>ZKDataViewWidget</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="54"/>
        <source>Connection Session:</source>
        <translation>连接Session：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="86"/>
        <source>●</source>
        <translation></translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="118"/>
        <source>Node Metadata</source>
        <translation>节点元数据</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="127"/>
        <source>AVersion:</source>
        <translation>ACL版本：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="134"/>
        <source>pzxid:</source>
        <translation>最后修改子节点事务ID：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="141"/>
        <source>CVersion:</source>
        <translation>子节点版本：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="162"/>
        <source>mzxid:</source>
        <translation>最后修改节点值事务ID：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="183"/>
        <source>czxid:</source>
        <translation>创建节点事务ID：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="211"/>
        <source>Number of Children:</source>
        <translation>子节点数量：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="218"/>
        <source>created time:</source>
        <translation>创建节点时间：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="225"/>
        <source>Data Length:</source>
        <translation>节点值的长度</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="239"/>
        <source>Version:</source>
        <translation>节点版本：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="267"/>
        <source>last modified time:</source>
        <translation>最后修改时间：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="274"/>
        <source>ephemeralOwner:</source>
        <translation>临时节点所有者：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="281"/>
        <source>Node Full Path:</source>
        <translation>节点完整路径：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="298"/>
        <source>Edit Node Data</source>
        <translation>编辑节点值</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="305"/>
        <source>[null]</source>
        <translation>[空数据（NULL）]</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.ui" line="345"/>
        <source>Done</source>
        <translation>操作完成</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="63"/>
        <source>Create New Child</source>
        <translation>新建子节点</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="64"/>
        <source>Delete This Node</source>
        <translation>删除这个节点</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="80"/>
        <source>Calling function: </source>
        <translation>正在调用方法：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="86"/>
        <source>Finish the function call: </source>
        <translation>完成方法调用：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="136"/>
        <location filename="ZKDataViewWidget.cpp" line="250"/>
        <location filename="ZKDataViewWidget.cpp" line="320"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="136"/>
        <source>Parent Node is nullptr. Now Path: </source>
        <translation>父节点不存在。当前节点路径：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="158"/>
        <source>Connected to ZooKeeper server.</source>
        <translation>已经连接到ZK服务器。</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="163"/>
        <source>Error when connecting to ZooKeeper server.</source>
        <translation>连接ZK服务器出错。</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="168"/>
        <source>Connecting...</source>
        <translation>正在连接......</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="223"/>
        <location filename="ZKDataViewWidget.cpp" line="294"/>
        <location filename="ZKDataViewWidget.cpp" line="345"/>
        <source>info</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="223"/>
        <source>Save value successfully.</source>
        <translation>保存节点数据成功。</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="294"/>
        <source>Create node successfully.</source>
        <translation>创建节点成功：</translation>
    </message>
    <message>
        <location filename="ZKDataViewWidget.cpp" line="345"/>
        <source>Delete node successfully.</source>
        <translation>删除节点成功：</translation>
    </message>
</context>
</TS>
