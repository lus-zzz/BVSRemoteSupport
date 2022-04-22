#include "FileServer.h"

#include <QFile>
#include <QSettings>
#include <signal.h>
#include <string.h>
#include <QDebug>
#include "mk_mediakit.h"

#ifdef _WIN32
#include "windows.h"
#else

#include "unistd.h"

#endif

#define LOG_LEV 4


/**
 * 收到http api请求广播(包括GET/POST)
 * @param parser http请求内容对象
 * @param invoker 执行该invoker返回http回复
 * @param consumed 置1则说明我们要处理该事件
 * @param sender http客户端相关信息
 */
//测试url : http://127.0.0.1/api/test
void API_CALL on_mk_http_request(const mk_parser parser,
                                 const mk_http_response_invoker invoker,
                                 int *consumed,
                                 const mk_sock_info sender) {

    char ip[64];
    log_printf(LOG_LEV,
               "client info, local: %s:%d, peer: %s:%d\n"
               "%s %s?%s %s\n"
               "User-Agent: %s\n"
               "%s",
               mk_sock_info_local_ip(sender,ip),
               mk_sock_info_local_port(sender),
               mk_sock_info_peer_ip(sender,ip + 32),
               mk_sock_info_peer_port(sender),
               mk_parser_get_method(parser),
               mk_parser_get_url(parser),
               mk_parser_get_url_params(parser),
               mk_parser_get_tail(parser),
               mk_parser_get_header(parser, "User-Agent"),
               mk_parser_get_content(parser,NULL));

    const char *url = mk_parser_get_url(parser);
    if(strcmp(url,"/api/test") != 0){
        *consumed = 0;
        return;
    }

    //只拦截api: /api/test
    *consumed = 1;
    const char *response_header[] = {"Content-Type","text/html",NULL};
    const char *content =
            "<html>"
            "<head>"
            "<title>hello world</title>"
            "</head>"
            "<body bgcolor=\"white\">"
            "<center><h1>hello world</h1></center><hr>"
            "<center>""ZLMediaKit-4.0</center>"
            "</body>"
            "</html>";
    mk_http_body body = mk_http_body_from_string(content,0);
    mk_http_response_invoker_do(invoker, 200, response_header, body);
    mk_http_body_release(body);
}

/**
 * 在http文件服务器中,收到http访问文件或目录的广播,通过该事件控制访问http目录的权限
 * @param parser http请求内容对象
 * @param path 文件绝对路径
 * @param is_dir path是否为文件夹
 * @param invoker 执行invoker返回本次访问文件的结果
 * @param sender http客户端相关信息
 */
void API_CALL on_mk_http_access(const mk_parser parser,
                                const char *path,
                                int is_dir,
                                const mk_http_access_path_invoker invoker,
                                const mk_sock_info sender) {

    char ip[64];
    log_printf(LOG_LEV,
               "client info, local: %s:%d, peer: %s:%d, path: %s ,is_dir: %d\n"
               "%s %s?%s %s\n"
               "User-Agent: %s\n"
               "%s",
               mk_sock_info_local_ip(sender, ip),
               mk_sock_info_local_port(sender),
               mk_sock_info_peer_ip(sender, ip + 32),
               mk_sock_info_peer_port(sender),
               path,(int)is_dir,
               mk_parser_get_method(parser),
               mk_parser_get_url(parser),
               mk_parser_get_url_params(parser),
               mk_parser_get_tail(parser),
               mk_parser_get_header(parser,"User-Agent"),
               mk_parser_get_content(parser,NULL));

    //有访问权限,每次访问文件都需要鉴权
    mk_http_access_path_invoker_do(invoker, NULL, NULL, 0);
}

/**
 * 在http文件服务器中,收到http访问文件或目录前的广播,通过该事件可以控制http url到文件路径的映射
 * 在该事件中通过自行覆盖path参数，可以做到譬如根据虚拟主机或者app选择不同http根目录的目的
 * @param parser http请求内容对象
 * @param path 文件绝对路径,覆盖之可以重定向到其他文件
 * @param sender http客户端相关信息
 */
void API_CALL on_mk_http_before_access(const mk_parser parser,
                                       char *path,
                                       const mk_sock_info sender) {

    char ip[64];
    log_printf(LOG_LEV,
               "client info, local: %s:%d, peer: %s:%d, path: %s\n"
               "%s %s?%s %s\n"
               "User-Agent: %s\n"
               "%s",
               mk_sock_info_local_ip(sender,ip),
               mk_sock_info_local_port(sender),
               mk_sock_info_peer_ip(sender,ip + 32),
               mk_sock_info_peer_port(sender),
               path,
               mk_parser_get_method(parser),
               mk_parser_get_url(parser),
               mk_parser_get_url_params(parser),
               mk_parser_get_tail(parser),
               mk_parser_get_header(parser, "User-Agent"),
               mk_parser_get_content(parser,NULL));
    //覆盖path的值可以重定向文件
}


FileServer::FileServer(QString port,QObject* parent):QObject(parent)
{
    QFile::copy(":/resources/c_api.ini","c_api.ini");
    QFile::setPermissions("c_api.ini",QFileDevice::ReadOther|QFileDevice::WriteOther);
    char *ini_path = mk_util_get_exe_dir("c_api.ini");
    mk_config config;
    config.ini = ini_path;
    config.ini_is_path = 1;
    config.log_level = 0;
    config.log_mask = LOG_CONSOLE;
    config.log_file_path = NULL;
    config.log_file_days = 0;
    config.ssl = NULL;
    config.ssl_is_path = 0;
    config.ssl_pwd = NULL;
    config.thread_num = 0;
    mk_env_init(&config);
    free(ini_path);

    mk_http_server_start(port.toInt(), 0);

    mk_events events;
    events.on_mk_http_request = on_mk_http_request;
    events.on_mk_http_access = on_mk_http_access;
    events.on_mk_http_before_access = on_mk_http_before_access;

    mk_events_listen(&events);

    log_info("media server %s", "stared!");
}

FileServer::~FileServer()
{
    mk_stop_all_server();
}
