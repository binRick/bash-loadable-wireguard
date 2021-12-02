#include "simpleconf.h"
#include "simpleconf.c"

int guard_config(int argc, char *argv[]) {
return 0;
    static const SimpleConfEntry sc_defs[] = {
        { "CONTAINER_MODE (<any*>)", "--CONTAINER_MODE=$0" },
        { "PRIVATE_IP_SERVICE (<any*>)", "--PRIVATE_IP_SERVICE=$0" },
        { "TERMINAL_PROFILE (<any*>)", "--TERMINAL_PROFILE=$0" },
        { "DEV_REGISTRY_SERVER_ENABLED (<any*>)", "--DEV_REGISTRY_SERVER_ENABLED=$0" },
        { "EXTRACE_SERVICE (<any*>)", "--EXTRACE_SERVICE=$0" },
        { "GO_CACHE_ENABLED (<any*>)", "--GO_CACHE_ENABLED=$0" },
        { "NPM_CACHE_ENABLED (<any*>)", "--NPM_CACHE_ENABLED=$0" },
        { "PKG_MANAGER_CACHE_ENABLED (<any*>)", "--PKG_MANAGER_CACHE_ENABLED=$0" },
        { "SSHD_LITE_SERVICE (<any*>)", "--SSHD_LITE_SERVICE=$0" },
        { "VPN_GUARD_SERVICE (<any*>)", "--VPN_GUARD_SERVICE=$0" },
        { "VPN_REST_SERVICE (<any*>)", "--VPN_REST_SERVICE=$0" },
        { "WG_API_SERVICE (<any*>)", "--WG_API_SERVICE=$0" },
        { "WORMHOLE_MAILBOX_SERVICE (<any*>)", "--WORMHOLE_MAILBOX_SERVICE=$0" },
        { "WORMHOLE_RELAY_SERVICE (<any*>)", "--WORMHOLE_RELAY_SERVICE=$0" },
    };
    sc_build_command_line_from_file("/etc/sysconfig/vpntech", NULL, sc_defs, 2,
                                    argv[0], &argc, &argv);
    fprintf(stderr,"argc=%d\n",argc);
    for (int i = 0; i < argc; i++){
      fprintf(stderr,"i=%d\n", i);
      fprintf(stderr,"argv[%d]=%s\n", i, argv[i]);

    }
    return 0;
}
int config_main(int argc, char *argv[]) {
    static const SimpleConfEntry sc_defs[] = {
        { "Name (<any*>)", "--name=$0" },
        { "Bell? <bool>",  "--bell"}
    };
    sc_build_command_line_from_file("hello.conf", NULL, sc_defs, 2,
                                    argv[0], &argc, &argv);
    fprintf(stderr,"argc=%d\n",argc);
    for (int i = 0; i < argc; i++){
      fprintf(stderr,"i=%d\n", i);
      fprintf(stderr,"argv[%d]=%s\n", i, argv[i]);

    }
    return 0;
}
