
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_CONFIG_H_INCLUDED_
#define _NGX_HTTP_CONFIG_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    /*
     *  指向一个指针数组，数组中的每个成员都是由所有HTTP模块的create_main_conf方法
     *  创建的存放全局配置项的结构体，他们存放着解析直属http{}块内的main级别的配置项参数
     */
    void        **main_conf;
    /*
     *  指向一个指针数组，数组中的每个成员都是由所有HTTP模块的create_srv_conf方法
     *  创建的与server相关的结构体
     */
    void        **srv_conf;
    /*
     *  指向一个指针数组，数组中的每个成员都是由所有HTTP模块的create_loc_conf方法
     *  创建的与location相关的结构体
     */
    void        **loc_conf;
} ngx_http_conf_ctx_t;


//每个HTTP模块必须实现的接口
typedef struct {
    //解析http{}内配置项前调用
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    //解析完http{}内所有配置项后调用
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    /*
     *  创建用于存储HTTP全局配置项的结构体，该结构体中的成员将保存直属于http{}块的配置项参数
     *  它会在解析main配置项前调用
     */
    void       *(*create_main_conf)(ngx_conf_t *cf);
    //解析完main配置项后回调
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    /*
     *  创建用于存储可同时出现在main、srv级别配置项的结构体，该结构体中的成员与
     *  server配置是相关联的
     */
    void       *(*create_srv_conf)(ngx_conf_t *cf);
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void       *(*create_loc_conf)(ngx_conf_t *cf);
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;


#define NGX_HTTP_MODULE           0x50545448   /* "HTTP" */

#define NGX_HTTP_MAIN_CONF        0x02000000
#define NGX_HTTP_SRV_CONF         0x04000000
#define NGX_HTTP_LOC_CONF         0x08000000
#define NGX_HTTP_UPS_CONF         0x10000000
#define NGX_HTTP_SIF_CONF         0x20000000
#define NGX_HTTP_LIF_CONF         0x40000000
#define NGX_HTTP_LMT_CONF         0x80000000


#define NGX_HTTP_MAIN_CONF_OFFSET  offsetof(ngx_http_conf_ctx_t, main_conf)
#define NGX_HTTP_SRV_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, srv_conf)
#define NGX_HTTP_LOC_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, loc_conf)


#define ngx_http_get_module_main_conf(r, module)                             \
    (r)->main_conf[module.ctx_index]
#define ngx_http_get_module_srv_conf(r, module)  (r)->srv_conf[module.ctx_index]
#define ngx_http_get_module_loc_conf(r, module)  (r)->loc_conf[module.ctx_index]


#define ngx_http_conf_get_module_main_conf(cf, module)                        \
    ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
#define ngx_http_conf_get_module_srv_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
#define ngx_http_conf_get_module_loc_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->loc_conf[module.ctx_index]

#define ngx_http_cycle_get_module_main_conf(cycle, module)                    \
    (cycle->conf_ctx[ngx_http_module.index] ?                                 \
        ((ngx_http_conf_ctx_t *) cycle->conf_ctx[ngx_http_module.index])      \
            ->main_conf[module.ctx_index]:                                    \
        NULL)


#endif /* _NGX_HTTP_CONFIG_H_INCLUDED_ */
