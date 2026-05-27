FROM python:3.11-slim

WORKDIR /app

# Install dependencies
COPY app/requirements.txt ./requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copy lambda handler packages.
# Directory is named 'lambda' — must create __init__.py to make it a package.
RUN mkdir -p lambda/GameMatchRequest lambda/GameMatchStatus lambda/GameRankReader
RUN touch lambda/__init__.py \
          lambda/GameMatchRequest/__init__.py \
          lambda/GameMatchStatus/__init__.py \
          lambda/GameRankReader/__init__.py

COPY lambda/GameMatchRequest/index.py       ./lambda/GameMatchRequest/index.py
COPY lambda/GameMatchRequest/requirements.txt ./lambda/GameMatchRequest/requirements.txt
COPY lambda/GameMatchStatus/index.py        ./lambda/GameMatchStatus/index.py
COPY lambda/GameMatchStatus/requirements.txt  ./lambda/GameMatchStatus/requirements.txt
COPY lambda/GameRankReader/index.py         ./lambda/GameRankReader/index.py
COPY lambda/GameRankReader/requirements.txt   ./lambda/GameRankReader/requirements.txt

COPY app/main.py ./main.py

EXPOSE 8000

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
