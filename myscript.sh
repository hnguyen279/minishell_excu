test1
test2

VAR="a b"
cat < "$VAR"    # safe
cat < $VAR      # ambiguous

check full quote because ambiguous appeart if not quote
cat < $VAR
-bash: $VAR: ambiguous redirect
hanguyen@DESKTOP-BBHTIEG:~$ cat < "$VAR"
-bash: A B: No such file or directory
hanguyen@DESKTOP-BBHTIEG:~$ cat < "  $VAR  "
-bash:   A B  : No such file or directory

hanguyen@DESKTOP-BBHTIEG:~$ unset VAR
hanguyen@DESKTOP-BBHTIEG:~$ cat < $VAR
-bash: $VAR: ambiguous redirect