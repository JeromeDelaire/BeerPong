#!/bin/bash

currentPath=$(pwd)

cd /lib/systemd/system
if [ -f pong-server.service ]; then
	rm pong-server.service
	echo 'pong-server.service deleted'
fi

echo 'creating pong-server.service...'
cat <<EOT >> pong-server.service
[Unit]
Description=Pong Server service

[Service]
WorkingDirectory=$currentPath/Raspberry
ExecStart=$currentPath/Raspberry/server
StandardOutput=journal
Restart=on-failure

[Install]
WantedBy=multi-user.target
Alias=pong-server.service
EOT

systemctl enable pong-server.service
systemctl start pong-server.service
