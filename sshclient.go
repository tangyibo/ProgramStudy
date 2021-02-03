package main

import (
	"fmt"
	"golang.org/x/crypto/ssh"
	"golang.org/x/crypto/ssh/terminal"
	"log"
	"os"
)

func main() {
	config := &ssh.ClientConfig{
		User: "root",
		Auth: []ssh.AuthMethod{
			ssh.Password("123456"),
		},
		HostKeyCallback: ssh.InsecureIgnoreHostKey(),
	}

	client, err := ssh.Dial("tcp", "192.168.1.130:22", config)
	if err != nil {
		log.Fatal("Failed to dial: ", err)
	}

	session, err := client.NewSession()

	if err != nil {
		panic(err)
	}

	defer session.Close()
	fd := int(os.Stdin.Fd())

	state, err := terminal.MakeRaw(fd)
	if err != nil {
		panic(err)
	}

	defer terminal.Restore(fd, state)

	session.Stdout = os.Stdout
	session.Stderr = os.Stderr
	session.Stdin = os.Stdin

	modes := ssh.TerminalModes{
		ssh.ECHO:          0,     // disable echoing
		ssh.TTY_OP_ISPEED: 14400, // input speed = 14.4kbaud
		ssh.TTY_OP_OSPEED: 14400, // output speed = 14.4kbaud
	}

	// Request pseudo terminal
	if err := session.RequestPty("xterm", 40, 80, modes); err != nil {
		log.Fatal("request for pseudo terminal failed: ", err)
	}

	if err := session.Shell(); err != nil {
		log.Fatal("failed to start shell: ", err)
	}

	fmt.Println(session.Wait())
}
