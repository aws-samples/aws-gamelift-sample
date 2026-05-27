"""
Locust load test for Gomoku backend API.

Usage:
    locust -f scripts/load_test.py --host <api_endpoint> --headless \
        -u 50 -r 5 -t 3m --csv results/scenario_a
    locust -f scripts/load_test.py --host <api_endpoint> --headless \
        -u 50 -r 5 -t 3m --csv results/scenario_b

Endpoints tested:
    POST /match           (40%) - start matchmaking
    GET  /match/{id}      (40%) - poll ticket status
    GET  /rank            (20%) - leaderboard read
"""

import random
import string
import threading

from locust import HttpUser, between, task

_ticket_lock = threading.Lock()
_tickets: list[str] = []


def _rand_name(length: int = 8) -> str:
    return "lt_" + "".join(random.choices(string.ascii_lowercase, k=length))


class GomokuUser(HttpUser):
    wait_time = between(0.5, 1.5)

    def on_start(self) -> None:
        self.player_name = _rand_name()
        self.player_pass = "loadtest"
        self.my_ticket: str | None = None

    @task(4)
    def match_request(self) -> None:
        payload = {"PlayerName": self.player_name, "PlayerPass": self.player_pass}
        with self.client.post(
            "/match",
            json=payload,
            name="POST /match",
            catch_response=True,
        ) as resp:
            if resp.status_code == 200:
                body = resp.json()
                tid = body.get("TicketId", "")
                if tid and tid not in ("AuthError", "MatchError"):
                    self.my_ticket = tid
                    with _ticket_lock:
                        _tickets.append(tid)
                    resp.success()
                else:
                    resp.failure(f"bad TicketId: {body}")
            else:
                resp.failure(f"HTTP {resp.status_code}")

    @task(4)
    def match_status(self) -> None:
        with _ticket_lock:
            tid = random.choice(_tickets) if _tickets else (self.my_ticket or "no-ticket")
        # Pass both PascalCase (API GW/Lambda) and snake_case (FastAPI/ECS) — each side
        # reads what it knows and ignores the rest.
        params = {
            "TicketId": tid,
            "ticket_id": tid,
            "PlayerName": self.player_name,
            "player_name": self.player_name,
        }
        with self.client.get(
            "/match",
            params=params,
            name="GET /match",
            catch_response=True,
        ) as resp:
            if resp.status_code == 200:
                resp.success()
            else:
                resp.failure(f"HTTP {resp.status_code}")

    @task(2)
    def rank_reader(self) -> None:
        with self.client.get("/rank", name="GET /rank", catch_response=True) as resp:
            if resp.status_code == 200:
                resp.success()
            else:
                resp.failure(f"HTTP {resp.status_code}")
