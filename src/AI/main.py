#!/usr/bin/env python3

##
## EPITECH PROJECT, 2025
## zappy_ai
## File description:
## Basic AI Client for Zappy
##

import socket
import time
import select
import sys

# Colors
BLUE = "\033[1;34m"
GREEN = "\033[1;32m"
YELLOW = "\033[1;33m"
RED = "\033[1;31m"
RESET = "\033[0m"

def log_info(msg):
    print(f"{BLUE}[INFO]{RESET} {msg}")

def log_send(msg):
    print(f"{GREEN}>>> {msg}{RESET}")

def log_recv(msg):
    print(f"{YELLOW}<<< {msg}{RESET}")

def log_error(msg):
    print(f"{RED}[ERROR]{RESET} {msg}")

def send(sock, message):
    sock.sendall((message + '\n').encode())
    log_send(message.strip())

def receive(sock):
    data = sock.recv(4096).decode()
    lines = data.strip().split("\n")
    for line in lines:
        log_recv(line.strip())
    return lines

def game_loop(sock):
    while True:
        time.sleep(1.0)
        # send(sock, "Inventory")
        send(sock, "Forward")
        lines = receive(sock)
        data = next((line for line in lines if line.startswith("[") and "food" in line), "")
        if not data:
            continue
        try:
            inv = data.replace("[", "").replace("]", "").split(", ")
            food_count = 0
            for item in inv:
                if "food" in item:
                    food_count = int(item.split()[1])
                    break
            if food_count < 5:
                log_info("Food is low, trying to take food.")
                send(sock, "Take food")
                receive(sock)
            else:
                log_info("Enough food. Moving forward.")
                send(sock, "Right")
                receive(sock)
                send(sock, "Forward")
                receive(sock)
        except Exception as e:
            log_error(f"Parsing error: {e}")


def main():
    if len(sys.argv) != 4:
        log_error("Usage: ./zappy_ai <machine> <port> <team_name>")
        return
    host, port, team = sys.argv[1], int(sys.argv[2]), sys.argv[3]
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((host, port))
            log_info("Connected to server.")
            welcome = receive(sock)
            if "WELCOME" not in welcome:
                log_error("Invalid server response.")
                return
            send(sock, team)
            sock.setblocking(0)
            start = time.time()
            while time.time() - start < 1.0:
                ready = select.select([sock], [], [], 0.1)
                if ready[0]:
                    receive(sock)
            sock.setblocking(1)
            game_loop(sock)
    except ConnectionRefusedError:
        log_error("Connection refused. Is the server running?")
    except Exception as e:
        log_error(f"Unexpected error: {e}")

if __name__ == "__main__":
    main()
