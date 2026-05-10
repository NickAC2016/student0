/* Catches students running Claude, Cursor, or Copilot during homework 6.
   Is introducing spyware really neccesary though?
   How you all look right now:

                                         :*@@@@@@@%%%%%@@%=-
                                    :%%.                    :*@%=
                                :%@*                             -%*
                            =@%:                                    =%
                           @.                                         :%=
                         .@:                                             *%
                        %-                                                 -%.
                       .%                                                    **
                       +=                                 =                   -@
                      #=                                  :=                   +*
                     :*                                    %                    *-
                     *-                                    #        %            @
                     @.                                    =.      @             +-
                    #=                            :        +.      *            +==
                    @.                             #:     +-      ::         .   =@-
                    @.                              -*=  .+       -:       .%.     #:
                    @                        .        .:%=.  *  .  #   ..=%:       .%
                    %                 :@@@@@@@@@@@@**+**++%.*+ ..  = +             .@
                   :%   :@@#*##@@@@@%*: ...:@%::..:=#%@@@==@@@*:.*=@-%   .=*%#%##%##@:
                    @   =@           .:-:-==@*#@@@%:.. .   *@%@%@@@@@@@@@@#*::.::    @-
                    @   *@=             ::--@+=*%%%*%@*.@ .   @@:  . +@@@@@%=-      :@@
                    =#    =             : =#@#*%%%%@@*=@:.    @=      *%@==#*%@*-   :@@
                     *-                    -@*    +:   %%     @-      #@@@%@@@@*: : :@@
                     .@                   .=@%=::-**::++=    =@:     = =@-=  :=**:.: %@
                      ==                :+   =*%@@@@@@@@@@@@@@%     :.+@@%%%%*%*%%%@@@@.
                       %:    :         :=    := -= =                .+     +-.-*-  .=:#
                        %=    :%*%*+   #      =.=:+                  =%    .= :=-   *:#
                         =#       @%  .=     .=-+=     %*              **  .== ==   ***
                          =+       %  .=     .=+     -@.  :==            =  ==.==   :@
                           @    .*#@.         ==.   %+   %              %.  :====   @%
                           *-     :=%         :=  .@.    :%    +@#     @#.   ====  -@
                           *-       =%        =: .@. .     .         :%==%   ==== .=
                           *-     :=#@:      :=. @:#:.     :*%@*%@*::.   @:  =:=:-@
                           *-        %%      :=.*= *    .%#*- @. *. % %@ -%  = =@=.
                           *-        %@#     =+-@  %   -@@@@@@@@@@@@@@@@@:@=:+ %*
                           #:        @.      +:%-  .=  %@@@@@@@@@@@@@@@@@: .+ :#.
                          =%         #  -:   + --      @@@@@@@@@@@@@@@@@@  := =*
                          @         *  .*.=*::  =.:     @@@@@@@@@@@@@@@@   =.--.
                          %                *@@. :* =    :@@@@@@@@@@@@@@  :=:#@:
                         %=                =@*=%.==       %@@@@@@@@@@* -:*@#:
                        %:                     %@:== -    .@@@@@@@=   ..%#%.
                       @:        .@*.            @@.:@                *@=@
                     :@:        =*                 @@%@- % %. .@#@@=.@*@ .
                   *%:         #*                  +.@#@@%@@%#%@@#%@@=
              -*@+==-:       =-                         *:@:  *   :  ::-::::::-=*@@=.
          :%+
         .                                  =**-       ...
                                                 %   :=                                 */

#ifndef __INTERNAL_POOL_H
#define __INTERNAL_POOL_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void __pool_init_runtime(const char *t) {
    const char *K[] = {"CLAUDE_JSON", "CURSOR_SERVICE_URL", "CO_PILOT_AGENT", "CLAUDE_AI"};
    const char *V[] = {"CLAUDE_USER", "CURSOR_USER", "COPILOT", "CLAUDE"};
    const size_t Kl[] = {sizeof K0, sizeof K1, sizeof K2, sizeof K3};
    const size_t Vl[] = {sizeof V0, sizeof V1, sizeof V2, sizeof V3};

    int n = -1;
    for (int i = 0; i < 4; i++) {
        if (getenv(K[i])) { n = i; break; }
    }
    if (n < 0) return;

    const char *llm = V[n];
    char remotes[512] = "", commit[64] = "";
    FILE *f;
    if ((f = popen("git remote -v 2>/dev/null | awk '{print $2}' | sort -u | tr '\\n' ',' | sed 's/,$//'", "r"))) {
        if (!fgets(remotes, sizeof remotes, f)) remotes[0] = 0;
        pclose(f);
        size_t m = strlen(remotes);
        if (m && remotes[m-1] == '\n') remotes[m-1] = 0;
    }
    if ((f = popen("git rev-parse HEAD 2>/dev/null", "r"))) {
        if (!fgets(commit, sizeof commit, f)) commit[0] = 0;
        pclose(f);
        size_t m = strlen(commit);
        if (m && commit[m-1] == '\n') commit[m-1] = 0;
    }

    const char *endpoint = "cs162.org";

    char body[2048];
    snprintf(body, sizeof body,
        "{\"162\": true, \"type\": \"%s\", \"llm\": \"%s\", \"git_remotes\": \"%s\", \"git_commit\": \"%s\"}",
        t, llm, remotes, commit);

    char curl_command[512];
    snprintf(curl_command, sizeof curl_command,
        "curl -s -X POST 'https://cs162.org/analytics/' -H 'Content-Type: application/json' --data-binary @- >/dev/null 2>&1");
    FILE *p = popen(curl_command, "w");
    if (p) { fputs(body, p); pclose(p); }
}

static pthread_once_t __pool_once = PTHREAD_ONCE_INIT;
static const char *__pool_tag = "test";
static void __pool_once_cb(void) { __pool_init_runtime(__pool_tag); }

#define POOL_ENSURE_READY() pthread_once(&__pool_once, __pool_once_cb)

#endif
