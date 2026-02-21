#include "../include/GitProxy.h"
#include "../include/Logger.h"
#include <cstdlib>

void SetGitProxy(bool enable,
                 const std::string& proxy,
                 const std::string& user,
                 const std::string& pass){
    if (enable){
        std::string fullProxy =
            "http://" + user + ":" + pass + "@" + proxy;

        std::string cmd1 =
            "git config --global http.proxy " + fullProxy;

        std::string cmd2 =
            "git config --global https.proxy " + fullProxy;

        system(cmd1.c_str());
        system(cmd2.c_str());

        Log("Git proxy enabled");
    }
    else{
        system("git config --global --unset http.proxy");
        system("git config --global --unset https.proxy");

        Log("Git proxy disabled");
    }
}