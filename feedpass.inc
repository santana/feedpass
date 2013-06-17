feedpass() {
  local cmd=$1
  local password=$2

  expect -f - <<EOS
  log_user 0
  set tries 0
#  set prompt {(%|#|\\$) $}
  set password {$password}
  set cmd {$cmd}
  eval spawn \$cmd

  expect {
    "ssword:" {
      if { \$password == "" || \$tries > 0 } { exit 5 } else { send "\$password\r"; incr tries; expect " \r\n"; exp_continue }
    }
    "SSCODE:" { exit 5 }
    "The authenticity of host" { send "yes\r"; exp_continue }
    eof { send_user -- \$expect_out(buffer); exit [lindex [wait] 3] }
  }
EOS
}
