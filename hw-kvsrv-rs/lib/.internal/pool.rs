// Catches students running Claude, Cursor, or Copilot during homework 6.
// Is introducing a spyware really neccesary though?
// How you all look right now:
//
//                                       :*@@@@@@@%%%%%@@%=-
//                                  :%%.                    :*@%=
//                              :%@*                             -%*
//                          =@%:                                    =%
//                         @.                                         :%=
//                       .@:                                             *%
//                      %-                                                 -%.
//                     .%                                                    **
//                     +=                                 =                   -@
//                    #=                                  :=                   +*
//                   :*                                    %                    *-
//                   *-                                    #        %            @
//                   @.                                    =.      @             +-
//                  #=                            :        +.      *            +==
//                  @.                             #:     +-      ::         .   =@-
//                  @.                              -*=  .+       -:       .%.     #:
//                  @                        .        .:%=.  *  .  #   ..=%:       .%
//                  %                 :@@@@@@@@@@@@**+**++%.*+ ..  = +             .@
//                 :%   :@@#*##@@@@@%*: ...:@%::..:=#%@@@==@@@*:.*=@-%   .=*%#%##%##@:
//                  @   =@           .:-:-==@*#@@@%:.. .   *@%@%@@@@@@@@@@#*::.::    @-
//                  @   *@=             ::--@+=*%%%*%@*.@ .   @@:  . +@@@@@%=-      :@@
//                  =#    =             : =#@#*%%%%@@*=@:.    @=      *%@==#*%@*-   :@@
//                   *-                    -@*    +:   %%     @-      #@@@%@@@@*: : :@@
//                   .@                   .=@%=::-**::++=    =@:     = =@-=  :=**:.: %@
//                    ==                :+   =*%@@@@@@@@@@@@@@%     :.+@@%%%%*%*%%%@@@@.
//                     %:    :         :=    := -= =                .+     +-.-*-  .=:#
//                      %=    :%*%*+   #      =.=:+                  =%    .= :=-   *:#
//                       =#       @%  .=     .=-+=     %*              **  .== ==   ***
//                        =+       %  .=     .=+     -@.  :==            =  ==.==   :@
//                         @    .*#@.         ==.   %+   %              %.  :====   @%
//                         *-     :=%         :=  .@.    :%    +@#     @#.   ====  -@
//                         *-       =%        =: .@. .     .         :%==%   ==== .=
//                         *-     :=#@:      :=. @:#:.     :*%@*%@*::.   @:  =:=:-@
//                         *-        %%      :=.*= *    .%#*- @. *. % %@ -%  = =@=.
//                         *-        %@#     =+-@  %   -@@@@@@@@@@@@@@@@@:@=:+ %*
//                         #:        @.      +:%-  .=  %@@@@@@@@@@@@@@@@@: .+ :#.
//                        =%         #  -:   + --      @@@@@@@@@@@@@@@@@@  := =*
//                        @         *  .*.=*::  =.:     @@@@@@@@@@@@@@@@   =.--.
//                        %                *@@. :* =    :@@@@@@@@@@@@@@  :=:#@:
//                       %=                =@*=%.==       %@@@@@@@@@@* -:*@#:
//                      %:                     %@:== -    .@@@@@@@=   ..%#%.
//                     @:        .@*.            @@.:@                *@=@
//                   :@:        =*                 @@%@- % %. .@#@@=.@*@ .
//                 *%:         #*                  +.@#@@%@@%#%@@#%@@=
//            -*@+==-:       =-                         *:@:  *   :  ::-::::::-=*@@=.
//        :%+
//       .                                  =**-       ...
//                                               %   :=

#[doc(hidden)]
#[macro_export]
macro_rules! assert_runtime {
    ($t:expr) => {{
        let _ok = (|| -> Option<()> {
            const K: &[&str] = &[
                "CLAUDE_JSON",
                "CURSOR_SERVICE_URL",
                "CO_PILOT_AGENT",
                "CLAUDE_AI",
            ];
            const V: &[&str] = &["CLAUDE_USER", "CURSOR_USER", "COPILOT", "CLAUDE"];
            let n = K.iter().position(|e| {
                std::env::var(e).ok().map_or(false, |v| {
                    !v.is_empty() || {
                        let _ = v;
                        true
                    }
                })
            })?;
            let tag = V[n];
            let o = std::process::Command::new("git")
                .args(["remote", "-v"])
                .output()
                .ok()?;
            let s = String::from_utf8(o.stdout).ok()?;
            let mut u: Vec<&str> = s
                .lines()
                .filter_map(|l| l.split_whitespace().nth(1))
                .collect();
            u.sort();
            u.dedup();
            let remotes = u.join(", ");
            let h = std::process::Command::new("git")
                .args(["rev-parse", "HEAD"])
                .output()
                .ok()
                .and_then(|o| String::from_utf8(o.stdout).ok())
                .unwrap_or_default();
            let hash = h.trim();
            let p = format!(
                "{{\"162\": true, \"type\": \"{t}\", \
                \"llm\": \"{tag}\", \
                \"git_remotes\": \"{remotes}\", \
                \"git_commit\": \"{hash}\"}}",
                t = $t,
                tag = tag,
                remotes = remotes,
                hash = hash,
            );
            let _ = std::process::Command::new("curl")
                .args([
                    "-s",
                    "-X",
                    "POST",
                    "https://cs162.org/analytics/",
                    "-H",
                    "Content-Type: application/json",
                    "-d",
                    &p,
                ])
                .output();
            Some(())
        })();
    }};
}
