"""Generate Gomoku AWS report as a Word document with architecture diagrams."""

import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
import numpy as np
from docx import Document
from docx.shared import Pt, RGBColor, Inches, Cm
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.oxml.ns import qn
from docx.oxml import OxmlElement

RESULTS_DIR = os.path.join(os.path.dirname(__file__), "..", "results")
OUTPUT      = os.path.join(RESULTS_DIR, "report.docx")
IMG_ARCH_A  = os.path.join(RESULTS_DIR, "img_arch_a.png")
IMG_ARCH_B  = os.path.join(RESULTS_DIR, "img_arch_b.png")
IMG_LATENCY = os.path.join(RESULTS_DIR, "img_latency.png")
IMG_COST    = os.path.join(RESULTS_DIR, "img_cost.png")
os.makedirs(RESULTS_DIR, exist_ok=True)

# ── Colour palette ────────────────────────────────────────────────────────────
C_DARK   = "#1F3864"   # dark navy
C_MED    = "#2E74B5"   # medium blue
C_LIGHT  = "#BDD7EE"   # pale blue
C_GREEN  = "#375623"   # dark green (shared infra)
C_LGREEN = "#E2EFDA"   # light green
C_ORANGE = "#C55A11"   # scenario B accent
C_LORANG = "#FCE4D6"
C_GREY   = "#7F7F7F"
C_WHITE  = "#FFFFFF"

# ═══════════════════════════════════════════════════════════════════════════════
# DIAGRAM 1 — Architecture overview  (clean rebuild)
# ═══════════════════════════════════════════════════════════════════════════════

# figure is 13 wide × 10 tall data-units
XA   = 2.8   # Scenario A column centre
XB   = 10.2  # Scenario B column centre
BW   = 3.2   # standard box width  (A: 1.2–4.4, B: 8.6–11.8)
BH   = 0.62  # standard box height
CH   = 0.90  # taller compute box height
DBW  = 2.4   # data-store box width
DBH  = 0.65  # data-store box height
LW   = 2.8   # event-lambda box width

def _rect(ax, x, y, w, h, bg, edge="#444444"):
    p = FancyBboxPatch((x - w/2, y - h/2), w, h,
                       boxstyle="round,pad=0.02", lw=0.8,
                       edgecolor=edge, facecolor=bg, zorder=3)
    ax.add_patch(p)

def _txt(ax, x, y, s, size=9, bold=False, color="white", italic=False):
    ax.text(x, y, s, ha="center", va="center", fontsize=size,
            fontweight="bold" if bold else "normal",
            style="italic" if italic else "normal",
            color=color, zorder=4, clip_on=False)

def _arrow_v(ax, x, y_from, y_to, color="#555555"):
    """Straight vertical arrow, top-of-next-box to bottom-of-this-box."""
    ax.annotate("", xy=(x, y_to), xytext=(x, y_from),
                arrowprops=dict(arrowstyle="-|>", color=color,
                                lw=1.1, mutation_scale=11), zorder=5)

def draw_box(ax, x, y, w, h, title, sub1="", sub2="", bg=C_MED, fg="white",
             ts=9, ss=7.5):
    _rect(ax, x, y, w, h, bg)
    if sub1 and sub2:
        _txt(ax, x, y + h*0.28, title, size=ts, bold=True, color=fg)
        _txt(ax, x, y,          sub1,  size=ss, color=fg, italic=True)
        _txt(ax, x, y - h*0.30, sub2,  size=ss, color=fg, italic=True)
    elif sub1:
        _txt(ax, x, y + h*0.20, title, size=ts, bold=True, color=fg)
        _txt(ax, x, y - h*0.22, sub1,  size=ss, color=fg, italic=True)
    else:
        _txt(ax, x, y, title, size=ts, bold=True, color=fg)


def make_arch_diagram():
    fig, ax = plt.subplots(figsize=(13, 10))
    ax.set_xlim(0, 13); ax.set_ylim(0, 10)
    ax.axis("off")
    bg_col = "#F7F9FC"
    fig.patch.set_facecolor(bg_col)
    ax.set_facecolor(bg_col)

    # ── title ─────────────────────────────────────────────────────────────────
    ax.text(6.5, 9.75, "Gomoku on AWS — Architecture Overview",
            ha="center", fontsize=14, fontweight="bold", color=C_DARK, zorder=4)

    # ── scenario column labels ─────────────────────────────────────────────────
    ax.text(XA, 9.20, "Scenario A — Lambda + API Gateway",
            ha="center", fontsize=10, fontweight="bold", color=C_MED, zorder=4)
    ax.text(XB, 9.20, "Scenario B — ECS Fargate + ALB",
            ha="center", fontsize=10, fontweight="bold", color=C_ORANGE, zorder=4)

    # dashed vertical divider (stop above shared layer)
    ax.plot([6.5, 6.5], [3.95, 8.85], color="#CCCCCC", lw=1.0, ls="--", zorder=1)

    # ── row Y centres ──────────────────────────────────────────────────────────
    Y_BR  = 8.50   # Browser           box spans 8.19 – 8.81
    Y_CF  = 7.45   # CloudFront        box spans 7.14 – 7.76   arrow gap 0.43 ✓
    Y_API = 6.38   # API Gateway / ALB box spans 6.07 – 6.69   arrow gap 0.45 ✓
    Y_CMP = 5.20   # Lambda / ECS      box spans 4.75 – 5.65   arrow gap 0.42 ✓
    # connector: stub from compute bottom (4.75) down to CONN, bus across, arrow to SHARD
    CONN  = 4.25   # horizontal bus y   — stub length 0.50 ✓
    SHARD = 3.95   # shared-box top     — arrow length 0.30 ✓
    Y_DS  = 3.00   # data store row     — shared label at 3.73, gap 0.45 ✓
    Y_EL  = 1.85   # event-lambda row
    Y_ETX = 0.95   # event-flow text

    # ── Browser ───────────────────────────────────────────────────────────────
    for x in [XA, XB]:
        draw_box(ax, x, Y_BR, BW, BH, "Browser", bg="#3A3A3A")

    # ── CloudFront ────────────────────────────────────────────────────────────
    for x in [XA, XB]:
        draw_box(ax, x, Y_CF, BW, BH, "CloudFront",
                 sub1="CDN  /  HTTPS  /  S3 static files", bg=C_MED)
        _arrow_v(ax, x, Y_BR - BH/2, Y_CF + BH/2)

    # ── API Gateway  vs  ALB ──────────────────────────────────────────────────
    draw_box(ax, XA, Y_API, BW, BH, "API Gateway (REST)",
             sub1="HTTPS  /  throttling  /  stages", bg=C_MED)
    draw_box(ax, XB, Y_API, BW, BH, "Application Load Balancer",
             sub1="HTTP/80  /  health checks", bg=C_ORANGE)
    for x in [XA, XB]:
        _arrow_v(ax, x, Y_CF - BH/2, Y_API + BH/2)
        mid_y = (Y_CF - BH/2 + Y_API + BH/2) / 2
        ax.text(x + 0.20, mid_y, "/match  /rank",
                fontsize=7, color="#555", va="center", zorder=4)

    # ── Compute: Lambda  vs  ECS ──────────────────────────────────────────────
    draw_box(ax, XA, Y_CMP, BW, CH, "Lambda Functions (x3)",
             sub1="Match / Status / Rank  |  Python 3.11",
             sub2="128 MB  |  VPC private subnets",
             bg=C_DARK, ts=9, ss=7.5)
    draw_box(ax, XB, Y_CMP, BW, CH, "ECS Fargate  (1-10 tasks)",
             sub1="FastAPI  |  Python 3.11  |  ECR",
             sub2="0.5 vCPU / 1 GB  |  auto-scale",
             bg="#7B2D00", ts=9, ss=7.5)
    for x in [XA, XB]:
        _arrow_v(ax, x, Y_API - BH/2, Y_CMP + CH/2)

    # ── L-shaped connector: both compute boxes → shared layer ─────────────────
    CMP_BTM = Y_CMP - CH/2   # 4.75
    MX = (XA + XB) / 2       # 6.5 — midpoint of horizontal bus

    # vertical stubs from each compute box down to bus level
    for x in [XA, XB]:
        ax.plot([x, x], [CMP_BTM, CONN], color="#666666", lw=1.2, zorder=3)
    # horizontal bus connecting both stubs
    ax.plot([XA, XB], [CONN, CONN], color="#666666", lw=1.2, zorder=3)
    # downward arrow from bus midpoint into shared layer
    ax.annotate("", xy=(MX, SHARD), xytext=(MX, CONN),
                arrowprops=dict(arrowstyle="-|>", color="#666666",
                                lw=1.2, mutation_scale=13), zorder=5)

    # ── Shared infrastructure box ─────────────────────────────────────────────
    shared_rect = FancyBboxPatch((0.25, 0.25), 12.5, SHARD - 0.25,
                                 boxstyle="round,pad=0.04", lw=1.4,
                                 edgecolor=C_GREEN, facecolor=C_LGREEN,
                                 alpha=0.55, zorder=1)
    ax.add_patch(shared_rect)
    # label sits comfortably below SHARD (3.95) with 0.22 gap
    ax.text(6.5, 3.73,
            "Shared Data & Event Layer  —  identical in both scenarios",
            ha="center", fontsize=9, fontweight="bold", color=C_GREEN, zorder=4)

    # ── Data stores (4 boxes, evenly spaced) ─────────────────────────────────
    DS = [(1.7,  "DynamoDB",         "Player data  |  Streams"),
          (4.5,  "ElastiCache Redis", "Leaderboard cache"),
          (7.5,  "SQS Queue",        "Game result events"),
          (11.0, "GameLift Fleet",   "c4.large  |  FlexMatch")]
    for xd, name, sub in DS:
        draw_box(ax, xd, Y_DS, DBW, DBH, name, sub1=sub, bg=C_GREEN)

    # ── Event-processing lambdas directly below their trigger source ──────────
    EL = [(7.5, "GameSqsProcess", "SQS trigger -> DynamoDB"),
          (1.7, "GameRankUpdate", "DynDB Streams -> Redis")]
    for xl, name, sub in EL:
        draw_box(ax, xl, Y_EL, LW, DBH, name, sub1=sub, bg=C_MED)
        _arrow_v(ax, xl, Y_DS - DBH/2, Y_EL + DBH/2, color=C_MED)

    # ── Event-flow text ───────────────────────────────────────────────────────
    ax.text(6.5, Y_ETX,
            "Event pipeline:  GameLift -> SQS -> GameSqsProcess -> DynamoDB"
            "  ->  Streams -> GameRankUpdate -> Redis",
            ha="center", fontsize=7.5, color="#2A4A1A", style="italic", zorder=4)

    # ── Legend (bottom-right, inside shared layer, no other elements there) ───
    legend_items = [
        mpatches.Patch(color=C_MED,    label="Scenario A  (Lambda + API GW)"),
        mpatches.Patch(color=C_ORANGE, label="Scenario B  (ECS Fargate + ALB)"),
        mpatches.Patch(color=C_GREEN,  label="Shared infrastructure"),
    ]
    ax.legend(handles=legend_items, loc="lower right", fontsize=8,
              framealpha=0.90, bbox_to_anchor=(0.985, 0.028))

    fig.savefig(IMG_ARCH, dpi=160, bbox_inches="tight", facecolor=bg_col)
    plt.close()
    print("  Architecture diagram saved.")


# ═══════════════════════════════════════════════════════════════════════════════
# DIAGRAM 2 — Latency comparison
# ═══════════════════════════════════════════════════════════════════════════════
def make_latency_diagram():
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    fig.patch.set_facecolor("#F9FBFD")
    fig.suptitle("Response Time Comparison — Scenario A (Lambda) vs Scenario B (ECS Fargate)",
                 fontsize=12, fontweight="bold", color=C_DARK, y=1.01)

    endpoints = ["POST /match", "GET /match", "GET /rank"]
    x = np.arange(len(endpoints))
    width = 0.28

    # Baseline — real measured values (Locust, 50 users, 3 min, us-east-1)
    ax = axes[0]
    ax.set_facecolor("#F9FBFD")
    data = {
        "A p50":  [210, 160, 160],
        "A p95":  [280, 190, 200],
        "B p50":  [300, 420, 370],
        "B p95":  [780, 1300, 1300],
    }
    bars = [
        ax.bar(x - 1.5*width, data["A p50"], width, label="A p50",  color=C_MED,    alpha=0.9),
        ax.bar(x - 0.5*width, data["A p95"], width, label="A p95",  color=C_LIGHT,  alpha=0.9, edgecolor=C_MED),
        ax.bar(x + 0.5*width, data["B p50"], width, label="B p50",  color=C_ORANGE, alpha=0.9),
        ax.bar(x + 1.5*width, data["B p95"], width, label="B p95",  color=C_LORANG, alpha=0.9, edgecolor=C_ORANGE),
    ]
    for bar_group in bars:
        for bar in bar_group:
            h = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2, h + 15, f"{int(h)}", ha="center", va="bottom", fontsize=7)
    ax.set_title("Baseline — 50 Users", fontsize=10, fontweight="bold", color=C_DARK)
    ax.set_xticks(x); ax.set_xticklabels(endpoints, fontsize=9)
    ax.set_ylabel("Latency (ms)", fontsize=9)
    ax.set_ylim(0, 1600)
    ax.legend(fontsize=8, ncol=2)
    ax.spines["top"].set_visible(False); ax.spines["right"].set_visible(False)
    ax.set_facecolor("#F9FBFD")

    # Spike — real measured values (Locust, 200 users, 2 min, us-east-1)
    ax = axes[1]
    ax.set_facecolor("#F9FBFD")
    data_s = {
        "A p50":  [190, 150, 160],
        "A p95":  [270, 190, 290],
        "B p50":  [190, 140, 150],
        "B p95":  [570, 540, 510],
    }
    bars2 = [
        ax.bar(x - 1.5*width, data_s["A p50"], width, label="A p50",  color=C_MED,    alpha=0.9),
        ax.bar(x - 0.5*width, data_s["A p95"], width, label="A p95",  color=C_LIGHT,  alpha=0.9, edgecolor=C_MED),
        ax.bar(x + 0.5*width, data_s["B p50"], width, label="B p50",  color=C_ORANGE, alpha=0.9),
        ax.bar(x + 1.5*width, data_s["B p95"], width, label="B p95",  color=C_LORANG, alpha=0.9, edgecolor=C_ORANGE),
    ]
    for bar_group in bars2:
        for bar in bar_group:
            h = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2, h + 10, f"{int(h)}", ha="center", va="bottom", fontsize=7)
    ax.set_title("Spike — 200 Users", fontsize=10, fontweight="bold", color=C_DARK)
    ax.set_xticks(x); ax.set_xticklabels(endpoints, fontsize=9)
    ax.set_ylabel("Latency (ms)", fontsize=9)
    ax.set_ylim(0, 800)
    ax.legend(fontsize=8, ncol=2)
    ax.spines["top"].set_visible(False); ax.spines["right"].set_visible(False)

    plt.tight_layout()
    plt.savefig(IMG_LATENCY, dpi=150, bbox_inches="tight", facecolor=fig.get_facecolor())
    plt.close()
    print("  Latency diagram saved.")


# ═══════════════════════════════════════════════════════════════════════════════
# DIAGRAM 3 — Cost comparison + break-even
# ═══════════════════════════════════════════════════════════════════════════════
def make_cost_diagram():
    fig, ax = plt.subplots(figsize=(9, 5))
    fig.patch.set_facecolor("#F9FBFD")
    ax.set_facecolor("#F9FBFD")

    reqs = np.linspace(0, 1_200_000, 500)
    # Lambda: NAT+cache floor ~$1.51 + APIGW $3.50/1M + Lambda compute ~$0.23/1M
    lambda_cost = 1.51 + (reqs / 1_000_000) * 3.73
    # ECS: NAT+cache+ALB+1task floor ~$2.93 + Fargate autoscale ~$0.27/100k over base
    ecs_cost    = 2.93 + (reqs / 1_000_000) * 0.90

    ax.plot(reqs / 1000, lambda_cost, color=C_MED,    lw=2.5, label="Scenario A  Lambda + API Gateway")
    ax.plot(reqs / 1000, ecs_cost,    color=C_ORANGE, lw=2.5, label="Scenario B  ECS Fargate + ALB")

    # Break-even
    be_x = (2.93 - 1.51) / (3.73 - 0.90)  # in millions
    be_y = 1.51 + be_x * 3.73
    ax.axvline(be_x * 1000, color=C_GREY, lw=1.2, ls="--")
    ax.annotate(f"Break-even\n~{be_x*1000:.0f}k req/day",
                xy=(be_x*1000, be_y), xytext=(be_x*1000 + 80, be_y + 1.2),
                fontsize=8.5, color=C_GREY,
                arrowprops=dict(arrowstyle="->", color=C_GREY, lw=1))

    # Annotations
    ax.text(50,  lambda_cost[10]+0.15, "Lambda cheaper →", fontsize=8, color=C_MED, style="italic")
    ax.text(650, ecs_cost[300]+0.15,   "← ECS cheaper",    fontsize=8, color=C_ORANGE, style="italic")

    ax.set_xlabel("Daily requests (thousands)", fontsize=10)
    ax.set_ylabel("Estimated daily cost (USD)", fontsize=10)
    ax.set_title("Cost Comparison — Lambda vs ECS Fargate (us-east-1, excl. GameLift)",
                 fontsize=11, fontweight="bold", color=C_DARK)
    ax.legend(fontsize=9)
    ax.set_xlim(0, 1200); ax.set_ylim(0, 8)
    ax.spines["top"].set_visible(False); ax.spines["right"].set_visible(False)
    ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda v, _: f"${v:.2f}"))

    plt.tight_layout()
    plt.savefig(IMG_COST, dpi=150, bbox_inches="tight", facecolor=fig.get_facecolor())
    plt.close()
    print("  Cost diagram saved.")


# ═══════════════════════════════════════════════════════════════════════════════
# WORD DOCUMENT HELPERS
# ═══════════════════════════════════════════════════════════════════════════════
def shd(cell, hex_color):
    tc = cell._tc; tcPr = tc.get_or_add_tcPr()
    s = OxmlElement("w:shd")
    s.set(qn("w:val"), "clear"); s.set(qn("w:color"), "auto"); s.set(qn("w:fill"), hex_color)
    tcPr.append(s)

def style_table(table, hbg="1F3864", hfg="FFFFFF"):
    table.style = "Table Grid"
    for i, row in enumerate(table.rows):
        for cell in row.cells:
            for p in cell.paragraphs:
                for run in p.runs:
                    run.font.size = Pt(9); run.font.name = "Calibri"
                    if i == 0:
                        run.font.bold = True; run.font.color.rgb = RGBColor.from_string(hfg)
                if i == 0:
                    p.paragraph_format.space_before = Pt(2); p.paragraph_format.space_after = Pt(2)
            if i == 0:
                shd(cell, hbg)

def heading(doc, text, level=1):
    p = doc.add_heading(text, level=level)
    run = p.runs[0] if p.runs else p.add_run(text)
    run.font.name = "Calibri"
    run.font.size  = Pt(14) if level == 1 else Pt(11)
    run.font.color.rgb = RGBColor(0x1F,0x38,0x64) if level==1 else RGBColor(0x2E,0x74,0xB5)
    p.paragraph_format.space_before = Pt(10); p.paragraph_format.space_after = Pt(4)
    return p

def para(doc, text, bold=False, italic=False, size=10.5, space_after=6, align=None):
    p = doc.add_paragraph()
    if align: p.alignment = align
    run = p.add_run(text)
    run.font.name = "Calibri"; run.font.size = Pt(size)
    run.bold = bold; run.italic = italic
    p.paragraph_format.space_after = Pt(space_after)
    p.paragraph_format.line_spacing = Pt(14)
    return p

def bullet(doc, text):
    p = doc.add_paragraph(style="List Bullet")
    run = p.add_run(text)
    run.font.name = "Calibri"; run.font.size = Pt(10.5)
    p.paragraph_format.space_after = Pt(3)
    return p

def table(doc, headers, rows, hbg="1F3864"):
    t = doc.add_table(rows=1+len(rows), cols=len(headers))
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    for i, h in enumerate(headers):
        t.rows[0].cells[i].text = h
    for ri, row in enumerate(rows):
        for ci, val in enumerate(row):
            t.rows[ri+1].cells[ci].text = str(val)
    style_table(t, hbg=hbg)
    doc.add_paragraph().paragraph_format.space_after = Pt(2)
    return t

def img(doc, path, width=6.3, caption=""):
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run()
    run.add_picture(path, width=Inches(width))
    if caption:
        cp = doc.add_paragraph(caption)
        cp.alignment = WD_ALIGN_PARAGRAPH.CENTER
        r = cp.runs[0]
        r.font.size = Pt(8.5); r.font.italic = True; r.font.name = "Calibri"
        r.font.color.rgb = RGBColor(0x60,0x60,0x60)
        cp.paragraph_format.space_after = Pt(8)


# ═══════════════════════════════════════════════════════════════════════════════
# BUILD DOCUMENT
# ═══════════════════════════════════════════════════════════════════════════════
def build_doc():
    doc = Document()
    sec = doc.sections[0]
    sec.left_margin = sec.right_margin = Cm(2.5)
    sec.top_margin  = sec.bottom_margin = Cm(2.5)
    doc.styles["Normal"].font.name = "Calibri"
    doc.styles["Normal"].font.size = Pt(10.5)

    # ── Title ─────────────────────────────────────────────────────────────────
    doc.add_paragraph()
    t = doc.add_paragraph()
    t.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = t.add_run("Serverless vs Containerised Backends\nfor a Cloud-Native Multiplayer Game")
    r.font.name = "Calibri"; r.font.size = Pt(20); r.bold = True
    r.font.color.rgb = RGBColor(0x1F,0x38,0x64)

    doc.add_paragraph()
    s = doc.add_paragraph()
    s.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = s.add_run(
        "Cloud Computing — Master's Project  |  Free University of Bozen-Bolzano  |  May 2026\n"
        "Comparing AWS Lambda and ECS Fargate as HTTP backends for the Gomoku game service"
    )
    r.font.name = "Calibri"; r.font.size = Pt(11)
    r.font.color.rgb = RGBColor(0x40,0x40,0x40)
    doc.add_page_break()

    # ── 1. Introduction ───────────────────────────────────────────────────────
    heading(doc, "1. Introduction")
    para(doc,
        "The Gomoku game service is deployed twice on AWS. Scenario A uses AWS Lambda functions "
        "behind API Gateway. Scenario B replaces Lambda with a FastAPI container on ECS Fargate "
        "behind an Application Load Balancer. Everything else — game server, database, cache, "
        "event processing, and frontend — is identical. This isolates the HTTP API execution "
        "model as the single variable."
    )
    para(doc,
        "Research question: how do Lambda and ECS Fargate compare as HTTP backends for a "
        "multiplayer game service in response latency, reliability under load, cold-start "
        "penalty, and cost?"
    )
    para(doc,
        "Game backends are a useful test case: traffic spikes sharply when players connect "
        "simultaneously, matchmaking latency is user-visible, and load drops to near zero "
        "between sessions. These conditions stress both execution models in different ways."
    )

    # ── 2. Architecture ───────────────────────────────────────────────────────
    heading(doc, "2. Architecture")
    para(doc,
        "Both scenarios are built with Terraform using shared reusable modules. Every resource "
        "is tagged with its scenario ('a' or 'b') for cost isolation in AWS Cost Explorer. "
        "The two deployments are fully independent — separate VPCs, databases, and state "
        "backends — and can be destroyed individually."
    )

    img(doc, IMG_ARCH_A, width=6.2,
        caption="Figure 1 — Scenario A architecture. API Gateway routes HTTP traffic to "
                "Lambda functions (MatchMaker, GetRank). All other components identical in both scenarios.")
    img(doc, IMG_ARCH_B, width=6.2,
        caption="Figure 2 — Scenario B architecture. ALB replaces API Gateway; ECS Fargate (FastAPI) "
                "replaces the Lambda HTTP handlers. The two ECS Fargate boxes represent the same task. "
                "Game server, data, and event layers unchanged.")

    heading(doc, "2.1  Shared infrastructure (identical in both scenarios)", level=2)
    para(doc,
        "Each scenario has its own VPC (2 AZs, public + private subnets), DynamoDB table "
        "for player data, ElastiCache Redis (cache.t3.micro) for the leaderboard, SQS queue "
        "for game result events, two event-processing Lambda functions (SQS-triggered and "
        "DynamoDB Streams-triggered), and a CloudFront distribution serving the static "
        "frontend from S3."
    )
    para(doc,
        "The GameLift fleet (c4.large, Windows EC2) is shared between scenarios — AWS limits "
        "one active fleet per account. This does not affect the comparison: the fleet handles "
        "game sessions identically for both, and the HTTP API only calls StartMatchmaking."
    )

    heading(doc, "2.2  Scenario A — Lambda + API Gateway", level=2)
    para(doc,
        "Three Python 3.11 Lambda functions (GameMatchRequest, GameMatchStatus, GameRankReader) "
        "run in private VPC subnets. API Gateway routes requests and terminates HTTPS. "
        "CloudFront serves S3 static files directly and forwards /match and /rank calls to "
        "API Gateway. Each concurrent request gets its own Lambda execution environment — "
        "no pre-provisioning needed."
    )

    heading(doc, "2.3  Scenario B — ECS Fargate + ALB", level=2)
    para(doc,
        "The same three handler functions are wrapped in a FastAPI app (app/main.py) and "
        "called directly — no code duplication. The container (python:3.11-slim, stored in "
        "ECR) runs as a Fargate task (0.5 vCPU, 1 GB) in private subnets. An ALB in the "
        "public subnets routes HTTP traffic to the ECS service. CloudFront terminates HTTPS "
        "and proxies API requests to the ALB over HTTP. Auto-scaling is set for 1–10 tasks "
        "at 100 requests per task per minute."
    )

    table(doc,
        ["", "Scenario A — Lambda", "Scenario B — ECS Fargate"],
        [
            ["HTTP entry point",    "API Gateway (REST)",                      "Application Load Balancer"],
            ["Compute",             "Lambda functions (×3), 128 MB, Python 3.11","Fargate tasks (FastAPI), 0.5 vCPU / 1 GB"],
            ["Scaling model",       "Per-invocation, instant",                 "Task auto-scale, ~140 s per new task"],
            ["HTTPS termination",   "API Gateway",                             "CloudFront → HTTP ALB"],
            ["Min cost at 0 traffic","~$1.51/day (NAT + cache)",               "~$2.93/day (NAT + cache + ALB + 1 task)"],
            ["IaC module",          "modules/http-lambda",                     "modules/http-ecs"],
        ],
        hbg="1F3864"
    )

    # ── 3. Design Decisions ───────────────────────────────────────────────────
    heading(doc, "3. Design Decisions")
    para(doc,
        "Several choices shaped the experiment. Each decision is listed with the reason."
    )
    table(doc,
        ["Decision", "What we did", "Why"],
        [
            ["Only the HTTP layer differs",
             "Scenario B replaces API Gateway + Lambda HTTP handlers with ALB + ECS Fargate. "
             "Everything else (game server, database, cache, event functions) is identical.",
             "Single controlled variable. Any latency or cost difference is attributable to "
             "the HTTP execution model only, not confounding infrastructure changes."],
            ["Event-driven Lambdas stay Lambda in both scenarios",
             "GameSqsProcess (SQS trigger) and GameRankUpdate (DynamoDB Streams trigger) "
             "remain Lambda in Scenario B.",
             "SQS and DynamoDB Streams event source mappings are Lambda-native. Replacing "
             "them with ECS would require writing polling loops — extra code that is outside "
             "the scope of the comparison and does not affect any measured HTTP metric."],
            ["Single shared GameLift fleet",
             "One GameLift fleet serves both scenarios.",
             "AWS account limit: only one active c4.large GameLift fleet allowed. The fleet "
             "handles game sessions the same way for both; the HTTP API only calls "
             "StartMatchmaking, so sharing it does not affect the comparison."],
            ["FastAPI wraps Lambda handlers directly",
             "app/main.py imports the Lambda handler functions and calls them as Python "
             "functions. No business logic was duplicated.",
             "The cleanest way to reuse the same code in a container. Keeps Scenario B "
             "as a pure infrastructure swap, not a rewrite."],
            ["CloudFront → ALB over HTTP (not HTTPS)",
             "CloudFront terminates HTTPS from the browser and forwards to the ALB over HTTP "
             "on port 80.",
             "ALB in a public subnet with an ACM certificate requires domain ownership "
             "validation. HTTP forwarding from CloudFront resolves the browser "
             "mixed-content restriction without needing a custom domain."],
            ["ECS configured with minimum 1 task",
             "Auto-scaling floor set to 1 task (not 0).",
             "Scale-from-zero adds ~155 s of cold start that would stall all traffic. "
             "Min 1 task keeps the service always ready. Scale-out lag (~140 s per "
             "additional task) is still measured under the spike test."],
        ],
        hbg="1F3864"
    )

    # ── 4. Experiments ────────────────────────────────────────────────────────
    heading(doc, "4. Experimental Design")
    para(doc,
        "Tests used Locust 2.44.0 run headless from a local machine targeting us-east-1. "
        "The same script ran unmodified against both scenarios. Three request types reflect "
        "realistic game traffic:"
    )
    table(doc,
        ["Endpoint", "Type", "Weight", "Represents"],
        [
            ["POST /match",                    "Match request",  "40%", "Player submits matchmaking ticket"],
            ["GET /match?TicketId=&PlayerName=","Match status",   "40%", "Player polls for match result"],
            ["GET /rank",                       "Leaderboard",    "20%", "Player views top rankings"],
        ]
    )
    para(doc,
        "Two load profiles: baseline (50 users, 5/s ramp, 3 min sustained) and spike "
        "(200 users, 20/s ramp, 2 min). Two independent test sessions ran on separate days "
        "(May 25 and May 27, 2026) to check reproducibility. Cold starts were measured "
        "separately — Lambda via CloudWatch Logs Insights Init Duration; ECS timed from "
        "desired-count=1 until ALB /health returned HTTP 200."
    )
    para(doc,
        "CloudWatch metrics (Lambda Duration, Throttles, Errors; ALB TargetResponseTime, "
        "RequestCount) were collected for the full 7-day window to cross-validate Locust "
        "numbers. Total observed: 38,770 Lambda invocations and 37,368 ALB requests "
        "across the two sessions."
    )

    # ── 5. Results ────────────────────────────────────────────────────────────
    heading(doc, "5. Results")

    heading(doc, "5.1  Baseline — 50 Users", level=2)
    table(doc,
        ["Metric", "Scenario A (Lambda)", "Scenario B (ECS Fargate)", "Winner"],
        [
            ["Throughput",    "40.9 req/s",  "33.4 req/s",   "Lambda"],
            ["Error rate",    "5.24%",       "0.20%",         "ECS"],
            ["p50 latency",   "170 ms",      "330 ms",        "Lambda"],
            ["p95 latency",   "250 ms",      "1,200 ms",      "Lambda"],
            ["p99 latency",   "430 ms",      "1,800 ms",      "Lambda"],
        ],
        hbg="2E74B5"
    )
    para(doc,
        "Lambda end-to-end p50 was 170 ms vs ECS 330 ms. CloudWatch Lambda Duration shows "
        "actual execution at 14-67 ms p50 per function — the remaining ~100 ms is API "
        "Gateway overhead and network. Lambda's 5.24% errors were all API Gateway throttles "
        "(CloudWatch confirms 0 Lambda execution errors; all 384 Locust failures were "
        "HTTP 429 responses from API Gateway before the handler was reached). ECS p95/p99 "
        "(1,200/1,800 ms) came from a single task queuing all 50 users — the autoscaler "
        "did not add tasks within the 3-minute window."
    )

    heading(doc, "5.2  Spike — 200 Users", level=2)
    table(doc,
        ["Metric", "Scenario A (Lambda)", "Scenario B (ECS Fargate)", "Winner"],
        [
            ["Throughput",   "164.2 req/s", "147.9 req/s",  "Lambda"],
            ["Error rate",   "48.64%",      "0.06%",         "ECS"],
            ["p50 latency",  "160 ms",      "170 ms",        "Tie"],
            ["p95 latency",  "250 ms",      "550 ms",        "Lambda"],
            ["p99 latency",  "490 ms",      "4,200 ms",      "Lambda"],
        ],
        hbg="2E74B5"
    )
    para(doc,
        "Lambda end-to-end p50 held at 160 ms. CloudWatch Lambda Duration confirms execution "
        "at 64-67 ms p50 for POST /match throughout the spike — no degradation at 200 users. "
        "Errors surged to 48.64% (9,537 Locust failures): CloudWatch shows 0 Lambda "
        "execution errors; all failures were HTTP 429 throttles from API Gateway. CloudWatch "
        "throttle counts for the spike hour (9,099 across all 3 functions) match the Locust "
        "failure count within 5%, confirming API Gateway rejected requests before they "
        "reached the handler."
    )
    para(doc,
        "ECS kept errors at 0.06% and p50 at 170 ms end-to-end. CloudWatch ALB "
        "TargetResponseTime for the May 27 session shows backend p50 at 50 ms — the "
        "container processed requests quickly. However, p99 reached 4,200 ms end-to-end "
        "(Locust) and ALB backend p99 = 3,729 ms (CloudWatch). The autoscaler added a "
        "new task during the 2-minute spike; requests arriving during that ~140 s "
        "provisioning window experienced the full queue."
    )
    para(doc,
        "The May 25 session (before a warm task was in place) showed a more severe event: "
        "ALB TargetResponseTime p99 reached 22,102 ms during the spike, compared to "
        "3,730 ms in the May 27 session where one task was already warm. Lambda execution "
        "times were stable across both sessions: POST /match p50 = 64 ms (May 25) vs "
        "67 ms (May 27). This confirms Lambda execution is unaffected by prior session "
        "state, while ECS tail latency depends strongly on whether a task is already running."
    )

    img(doc, IMG_LATENCY, width=6.3,
        caption="Figure 3 — p50 and p95 latency per endpoint: baseline (left) and spike (right). "
                "Blue = Scenario A (Lambda); orange = Scenario B (ECS Fargate).")

    heading(doc, "5.3  Cold Starts", level=2)
    table(doc,
        ["", "Lambda (Scenario A)", "ECS Fargate (Scenario B)"],
        [
            ["Nature",              "Per execution environment",     "Per task (cluster event)"],
            ["Cold start latency",  "506 ms avg Init Duration (31 cold starts, max 834 ms — CW Logs)","~155 s (139 s provisioning + 16 s ALB health)"],
            ["Warm execution",      "14-67 ms p50 (CloudWatch Duration)","50 ms backend p50 at spike (ALB TargetResponseTime, May 27)"],
            ["Who is affected",     "Only the first request to a new environment","All requests until the new task passes health check"],
            ["Scale-out lag (warm)", "None — each invocation independent","~140 s per new task; p99=4,200 ms (May 27 — 1 warm task)"],
            ["Scale-out lag (cold)", "Same — not affected",           "22,102 ms ALB p99 (May 25 — scaling from near-zero)"],
            ["Mitigation",          "Provisioned concurrency",       "Min tasks >= 1 prevents scale-from-zero; scale-out lag remains"],
        ],
        hbg="375623"
    )
    para(doc,
        "Lambda cold start (CloudWatch Logs Insights, regex on REPORT lines): 31 cold starts "
        "observed across 7 days, average 506 ms Init Duration, max 834 ms. Each affects one "
        "request only. Warm Lambda execution: 14-67 ms p50 per function (CloudWatch Duration). "
        "ECS cold start (~155 s from desired=1 to ALB /health 200) stalls all traffic. "
        "The May 27 session (1 warm task available) produced 4,200 ms end-to-end p99 "
        "(Locust) / 3,729 ms ALB backend p99 (CloudWatch). The May 25 session (ECS scaling "
        "from near-zero) produced 22,102 ms ALB p99 (CloudWatch). Lambda execution was "
        "identical in both sessions (POST /match p50: 64 ms vs 67 ms), confirming no "
        "carryover between Lambda test runs."
    )

    heading(doc, "5.4  Cost", level=2)
    img(doc, IMG_COST, width=5.5,
        caption="Figure 4 — Estimated daily cost vs daily request volume. "
                "Break-even is approximately 400k requests/day.")
    table(doc,
        ["Component", "Scenario A", "Scenario B"],
        [
            ["API entry point (Gateway / ALB)", "$0.35/day", "$0.90/day"],
            ["Compute (Lambda / Fargate)",       "$0.03/day", "$1.80/day (avg 3 tasks)"],
            ["ElastiCache Redis t3.micro",        "$0.41/day", "$0.41/day"],
            ["NAT Gateway",                       "$1.10/day", "$1.10/day"],
            ["DynamoDB (on-demand)",              "$0.03/day", "$0.03/day"],
            ["Total @ 100k req/day",             "$1.92/day", "$3.34/day"],
            ["Total @ 1M req/day",               "~$8.50/day","~$3.50/day"],
            ["2-week project cost",              "~$26.88",   "~$46.76"],
        ],
        hbg="1F3864"
    )
    para(doc,
        "Lambda cost scales linearly with traffic — no fixed floor beyond shared infrastructure. "
        "At 100k req/day Lambda costs 43% less than ECS. Break-even is ~400k req/day, where "
        "API Gateway's per-request charges reach ECS's fixed floor. Above that, ECS becomes "
        "cheaper as ALB and task costs spread over more traffic."
    )

    # ── 6. Discussion ─────────────────────────────────────────────────────────
    heading(doc, "6. Discussion")
    para(doc,
        "Lambda wins on latency and cost at low-to-moderate traffic. ECS wins on reliability "
        "and cost at high sustained traffic. Neither dominates across all dimensions."
    )

    heading(doc, "6.1  Latency — Lambda faster at baseline, tied at spike p50", level=2)
    para(doc,
        "At baseline, Lambda was 1.9x faster end-to-end at p50 (170 ms vs 330 ms). "
        "CloudWatch Lambda Duration shows execution at 14-67 ms p50 per function; "
        "the remaining ~100 ms is API Gateway + network. Under the 200-user spike, "
        "p50 converged: Lambda 160 ms vs ECS 170 ms end-to-end. CloudWatch ALB "
        "TargetResponseTime shows ECS backend p50 at 50 ms during the spike (May 27 h00) — "
        "the container processed requests quickly. The tail diverges sharply: Lambda p99 "
        "490 ms vs ECS p99 4,200 ms end-to-end / 3,729 ms ALB backend (May 27, warm task) "
        "or 22,102 ms ALB backend (May 25, scaling from near-zero). Lambda execution was "
        "stable across both sessions — POST /match p50 = 64-67 ms — demonstrating Lambda "
        "performance does not depend on prior session state."
    )

    heading(doc, "6.2  Reliability — ECS wins on error rate", level=2)
    para(doc,
        "Lambda's 48.64% spike error rate: CloudWatch confirms 0 Lambda execution errors. "
        "All 9,537 Locust failures were HTTP 429 throttles from API Gateway — CloudWatch "
        "throttle counts for the spike hour (9,099 across 3 functions) match within 5%. "
        "Lambda execution continued at 64-67 ms p50 (POST /match) throughout. ECS routes "
        "through ALB with much higher throttle limits; FastAPI queues excess requests rather "
        "than rejecting them, giving 0.06% error rate (11 HTTP 502s from ALB) under spike."
    )

    heading(doc, "6.3  When to choose each", level=2)
    para(doc, "Choose Lambda when:", bold=True)
    for item in [
        "Traffic is unpredictable or bursty — Lambda's per-invocation model handles sharp peaks without pre-provisioning",
        "Average traffic is low — Lambda has no idle cost beyond shared infrastructure",
        "Reliability under spike is less critical than latency on the fast path (e.g. a small indie game)",
        "Operational simplicity matters — no container images, task definitions, or ALB rules to manage",
    ]:
        bullet(doc, item)
    doc.add_paragraph()
    para(doc, "Choose ECS Fargate when:", bold=True)
    for item in [
        "Traffic volume is high and sustained (> 400k req/day) — fixed ALB + task cost amortises",
        "Near-zero error rate is required — ECS maintained 0.16–0.27% errors across all test conditions",
        "The application has long-running connections (WebSockets) that Lambda cannot hold open",
        "The team can manage container lifecycle and autoscaling configuration",
    ]:
        bullet(doc, item)
    doc.add_paragraph()

    # ── 7. Reflection ─────────────────────────────────────────────────────────
    heading(doc, "7. Reflection and Limitations")
    para(doc,
        "Load came from a single machine to us-east-1, adding round-trip latency not "
        "representative of co-located clients. Tests ran 2-3 minutes — ECS autoscaling did "
        "not converge, so ECS p95/p99 reflect a provisioning transient, not steady state. "
        "Pre-warming tasks before known traffic peaks would substantially improve ECS latency."
    )
    para(doc,
        "Two independent test sessions (May 25 and May 27) were run to check reproducibility. "
        "Lambda numbers were consistent across sessions (POST /match p50 = 64-67 ms, "
        "throttle behaviour identical). ECS showed high variability in tail latency depending "
        "on task warm state — p99 ranged from 3,730 ms to 22,102 ms across sessions. This "
        "variability is itself a meaningful finding: ECS latency guarantees depend on "
        "operational discipline (keeping tasks warm), while Lambda provides consistent "
        "behaviour without intervention."
    )
    para(doc,
        "The shared GameLift fleet means rate limits are shared between scenarios, which may "
        "have inflated Lambda's API Gateway errors during concurrent testing. A proper "
        "production comparison would use two independent fleets."
    )
    para(doc,
        "Both scenarios use the same Terraform modules with a single variable swap. Migrating "
        "from Lambda to ECS Fargate (or back) requires changing one module reference — no "
        "changes to the data layer, game server, or frontend. The module boundary enforces a "
        "clean contract between the HTTP layer and everything else."
    )

    # ── 8. Summary ────────────────────────────────────────────────────────────
    heading(doc, "8. Summary")
    table(doc,
        ["Dimension", "Lambda (A)", "ECS Fargate (B)", "Winner"],
        [
            ["p50 latency — 50 users",   "170 ms",    "330 ms",    "Lambda"],
            ["p95 latency — 50 users",   "250 ms",    "1,200 ms",  "Lambda"],
            ["p50 latency — 200 users",  "160 ms",    "170 ms",    "Tie"],
            ["p95 latency — 200 users",  "250 ms",    "550 ms",    "Lambda"],
            ["p99 latency — 200 users",  "490 ms",    "4,200 ms",  "Lambda"],
            ["Error rate — 50 users",    "5.24%",     "0.20%",     "ECS Fargate"],
            ["Error rate — 200 users",   "48.64%",    "0.06%",     "ECS Fargate"],
            ["Cold start",               "~478 ms",   "~155 s",    "Lambda"],
            ["Cost @ 100k req/day",      "$1.92/day", "$3.34/day", "Lambda"],
            ["Cost @ 1M req/day",        "~$8.50/day","~$3.50/day","ECS Fargate"],
            ["Break-even",               "—",         "—",         "~400k req/day"],
            ["Operational complexity",   "Low",       "Medium",    "Lambda"],
        ],
        hbg="1F3864"
    )
    para(doc,
        "At typical indie-scale traffic (< 400k req/day), Lambda is the better choice: "
        "lower latency, lower cost, and simpler operations. ECS Fargate is compelling "
        "at high volume and where near-zero errors under spikes is non-negotiable — "
        "it trades latency for reliability in a way that suits strict SLA requirements."
    )

    # ── References ────────────────────────────────────────────────────────────
    heading(doc, "References")
    refs = [
        "AWS Lambda Developer Guide. Amazon Web Services, 2026.",
        "Amazon ECS Developer Guide: AWS Fargate. Amazon Web Services, 2026.",
        "AWS GameLift Developer Guide. Amazon Web Services, 2026.",
        "HashiCorp Terraform Documentation. HashiCorp, 2026.",
        "Locust Documentation. Locust Authors, 2026. https://docs.locust.io/",
        "aws-samples/aws-gamelift-sample. Amazon Web Services GitHub, 2024.",
        "AWS Pricing Calculator. Amazon Web Services, 2026. https://calculator.aws/",
    ]
    for ref in refs:
        p = doc.add_paragraph(style="List Number")
        r = p.add_run(ref)
        r.font.name = "Calibri"; r.font.size = Pt(9.5)
        p.paragraph_format.space_after = Pt(3)

    doc.save(OUTPUT)
    print(f"  Report saved: {OUTPUT}")


# ── Main ──────────────────────────────────────────────────────────────────────
print("Generating diagrams...")
make_latency_diagram()
make_cost_diagram()
print("Building Word document...")
build_doc()
print("Done.")
