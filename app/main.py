import sys
import os
import json
import importlib

# Add project root so handler packages are importable.
# Note: the directory is named 'lambda' (a Python keyword) so we use importlib.
_PROJECT_ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _PROJECT_ROOT)

from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse

# importlib.import_module works fine with dotted paths even when the top-level
# package name is a reserved word, as long as the path is on sys.path.
_match_request_mod = importlib.import_module("lambda.GameMatchRequest.index")
_match_status_mod  = importlib.import_module("lambda.GameMatchStatus.index")
_rank_reader_mod   = importlib.import_module("lambda.GameRankReader.index")

_match_request = _match_request_mod.lambda_handler
_match_status  = _match_status_mod.lambda_handler
_rank_reader   = _rank_reader_mod.handler

app = FastAPI(title="Gomoku Backend")


def _proxy_event(method: str, path_params: dict | None = None, body: dict | None = None) -> dict:
    return {
        "httpMethod": method,
        "pathParameters": path_params or {},
        "body": json.dumps(body) if body else None,
        "headers": {},
        "queryStringParameters": {},
    }


@app.get("/health")
def health():
    return {"status": "ok"}


@app.post("/match")
async def match_request(request: Request):
    body = await request.json()
    result = _match_request(_proxy_event("POST", body=body), None)
    return JSONResponse(
        content=json.loads(result["body"]),
        status_code=result.get("statusCode", 200),
    )


@app.get("/match")
def match_status(ticket_id: str = "", player_name: str = ""):
    event = _proxy_event("GET")
    event["queryStringParameters"] = {"TicketId": ticket_id, "PlayerName": player_name}
    result = _match_status(event, None)
    return JSONResponse(
        content=json.loads(result["body"]),
        status_code=result.get("statusCode", 200),
    )


@app.get("/rank")
def rank_reader():
    result = _rank_reader(_proxy_event("GET"), None)
    return JSONResponse(
        content=json.loads(result["body"]),
        status_code=result.get("statusCode", 200),
    )
