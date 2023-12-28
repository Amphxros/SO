#!/bin/bash

# Leemos el fichero /etc/passwd y mostramos cada entrada
while IFS=':' read -r login_name optional_encrypted_passwd uid gid user_name user_home user_shell; do
    # Mostramos solo las entradas donde el home del usuario es un subdirectorio de /home
    home_directory=$(dirname "$user_home")
    if [ "$home_directory" == "/home" ] || [[ "$home_directory" == "/home/"* ]]; then
        # Mostramos la entrada con formato similar a show-passwd
        printf "[Entry]\n\tlogin=%s\n\tenc_pass=%s\n\tuid=%d\n\tgid=%d\n\tuser_name=%s\n\t" \
               "home=%s\n\tshell=%s\n" \
               "$login_name" "$optional_encrypted_passwd" "$uid" "$gid" "$user_name" "$user_home" "$user_shell"
    fi
done < /etc/passwd