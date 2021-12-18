# Re-tarscpp

看看Tars

---

- Tars: https://github.com/TarsCloud/TarsCpp
- 文档：https://tarscloud.gitbook.io/

**架构**

![f](https://tarscloud.gitbook.io/~/files/v0/b/gitbook-x-prod.appspot.com/o/spaces%2F-LjKWVpRE8j8fOTPPSus%2Fuploads%2Fgit-blob-18429215ed75f04b9f18906d0a4bf607dc5b297f%2Ftars_tuopu.png?alt=media)

**交互**

![c](https://tarscloud.gitbook.io/~/files/v0/b/gitbook-x-prod.appspot.com/o/spaces%2F-LjKWVpRE8j8fOTPPSus%2Fuploads%2Fgit-blob-86462d5e2cce743a866d04c4dcfddde92338f908%2Ftars_jiaohu.png?alt=media)

---

**服务架构**

![sf](https://tarscloud.gitbook.io/~/files/v0/b/gitbook-x-prod.appspot.com/o/spaces%2F-LjKWVpRE8j8fOTPPSus%2Fuploads%2Fgit-blob-e380cf1ce5fde20fa2ac6cf71322bdf1507ca200%2Ftars_server_client%20(1).png?alt=media)

```
服务端：
NetThread： 收发包，连接管理，多线程(可配置），采用epoll ET触发实现，支持tcp/udp；
BindAdapter： 绑定端口类，用于管理Servant对应的绑定端口的信息操作；
ServantHandle：业务线程类，根据对象名分派Servant的对象和接口调用；
AdminServant： 管理端口的对象；
ServantImp： 继承Servant的业务处理基类（Servant：服务端接口对象的基类）；
```

```
客户端：
NetThread： 收发包，连接管理，多线程(可配置），采用epoll ET触发实现，支持tcp/udp；
AdapterProxy： 具体服务器某个节点的本地代理，管理到服务器的连接，以及请求超时处理；
ObjectProxy： 远程对象代理，负责路由分发、负载均衡、容错，支持轮询/hash/权重；
ServantProxy： 远程对象调用的本地代理，支持同步/异步/单向，Tars协议和非Tars协议；
AsyncThread： 异步请求的回应包处理线程；
Callback： 具体业务Callback的处理基类对象。
```
---

- 分析: 
  
    - https://github.com/Myicefrog/learn-tars
      
    - https://blog.csdn.net/stpeace/category_7554860.html
      
    - https://my.oschina.net/u/4021301/blog/4394168
