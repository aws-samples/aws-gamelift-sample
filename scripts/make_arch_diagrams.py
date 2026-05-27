"""
Generate Scenario A and Scenario B architecture diagrams from the original repo image.
Scenario A: original with title banner.
Scenario B: replace API Gateway → ALB, MatchMaker Lambda → ECS Fargate, GetRank Lambda → ECS Fargate.
"""

from PIL import Image, ImageDraw, ImageFont
import os

SRC   = os.path.join(os.path.dirname(__file__),
        "../aws-gamelift-sample/docs/en/images/Architecture.png")
OUT_A = os.path.join(os.path.dirname(__file__), "../results/img_arch_a.png")
OUT_B = os.path.join(os.path.dirname(__file__), "../results/img_arch_b.png")

# ── Fonts ─────────────────────────────────────────────────────────────────────
def _font(name, size):
    paths = [f"C:/Windows/Fonts/{name}", f"/usr/share/fonts/truetype/dejavu/{name}"]
    for p in paths:
        if os.path.exists(p):
            return ImageFont.truetype(p, size)
    return ImageFont.load_default()

F_TITLE  = _font("calibrib.ttf", 17)
F_BOLD   = _font("calibrib.ttf", 13)
F_MED    = _font("calibri.ttf",  11)
F_SMALL  = _font("calibri.ttf",  10)

# ── Colours ───────────────────────────────────────────────────────────────────
WHITE   = (255, 255, 255, 255)
ALB_BG  = (232, 119,  34, 255)   # AWS orange (matches Lambda icons in original)
ALB_BRD = (170,  80,  10, 255)
ECS_BG  = ( 18, 104, 176, 255)   # AWS ECS blue
ECS_BRD = ( 10,  70, 130, 255)
DASH_C  = ( 18, 104, 176, 180)   # dashed connector colour

BANNER_A = ( 31,  56, 100, 255)  # dark navy
BANNER_B = (180,  70,   5, 255)  # dark orange

BANNER_H = 38

# ── Helpers ───────────────────────────────────────────────────────────────────
def add_banner(img: Image.Image, text: str, color: tuple) -> Image.Image:
    """Prepend a coloured title banner above the image."""
    out = Image.new("RGBA", (img.width, img.height + BANNER_H), WHITE)
    draw = ImageDraw.Draw(out)
    draw.rectangle([(0, 0), (img.width, BANNER_H)], fill=color)
    draw.text((img.width // 2, BANNER_H // 2), text,
              fill=WHITE, anchor="mm", font=F_TITLE)
    out.paste(img, (0, BANNER_H))
    return out


def service_box(draw: ImageDraw.ImageDraw,
                x1, y1, x2, y2,
                line1: str, line2: str, line3: str,
                label_below: str,
                bg, border):
    """Draw an icon-replacement box + label below it."""
    cx = (x1 + x2) // 2

    # filled rounded rectangle (icon area)
    draw.rounded_rectangle([(x1, y1), (x2, y2)],
                            radius=7, fill=bg, outline=border, width=2)

    # text inside box — centre vertically across three lines
    total_lines = sum(bool(t) for t in [line1, line2, line3])
    spacing = 14
    box_cy = (y1 + y2) // 2
    start_y = box_cy - (total_lines - 1) * spacing // 2

    row = 0
    for txt in [line1, line2, line3]:
        if txt:
            draw.text((cx, start_y + row * spacing), txt,
                      fill=WHITE, anchor="mm",
                      font=F_BOLD if row == 0 else F_MED)
            row += 1

    # label text below box
    if label_below:
        draw.text((cx, y2 + 13), label_below,
                  fill=(50, 50, 50, 255), anchor="mm", font=F_MED)


def dashed_hline(draw, x1, x2, y, color, dash=8, gap=5):
    x = x1
    while x < x2:
        draw.line([(x, y), (min(x + dash, x2), y)], fill=color, width=2)
        x += dash + gap


def dashed_vline(draw, x, y1, y2, color, dash=8, gap=5):
    y = y1
    while y < y2:
        draw.line([(x, y), (x, min(y + dash, y2))], fill=color, width=2)
        y += dash + gap


# ── Precise erase regions (covers icon + arrow stubs, NOT the through-arrows) ─
#
# From the 816×577 grid image:
#
#   API Gateway  icon ≈ (52, 178)–(118, 257)   label ≈ y 263–287
#   MatchMaker   icon ≈ (268, 178)–(343, 258)  label ≈ y 263–287
#   GetRank      icon ≈ (38, 392)–(113, 464)   label ≈ y 465–491
#
# Arrows to preserve:
#   • Web-page → API GW: enters icon top around x=85  → leave ≥ 4px gap at top
#   • API GW → GetRank: exits icon bottom around x=85 → leave ≥ 4px gap at bottom
#   • Web-page → MatchMaker: enters icon top around x=305 → leave gap at top
#   • MatchMaker → GameLift: exits icon right around y=218 → leave gap at right

# Erase rectangles: slightly inset from arrow entry/exit edges
ERASE_APIGW    = (32,  181, 143, 291)   # label cleared down to 291
ERASE_MATCHMK  = (260, 181, 352, 291)
ERASE_GETRANK  = (26,  394, 128, 494)


def erase(draw, rect):
    draw.rectangle([rect[:2], rect[2:]], fill=WHITE)


# ═════════════════════════════════════════════════════════════════════════════
# SCENARIO A — original + banner only
# ═════════════════════════════════════════════════════════════════════════════
img_a = Image.open(SRC).convert("RGBA")
img_a = add_banner(img_a, "Scenario A — Lambda + API Gateway", BANNER_A)
img_a.save(OUT_A, dpi=(160, 160))
print(f"  Saved {OUT_A}")


# ═════════════════════════════════════════════════════════════════════════════
# SCENARIO B — replace HTTP-layer components
# ═════════════════════════════════════════════════════════════════════════════
img_b = Image.open(SRC).convert("RGBA")
draw  = ImageDraw.Draw(img_b)

# 1. Erase API Gateway ─────────────────────────────────────────────────────
erase(draw, ERASE_APIGW)
service_box(draw,
    x1=38,  y1=182, x2=128, y2=258,
    line1="ALB", line2="App Load", line3="Balancer",
    label_below="App Load Balancer",
    bg=ALB_BG, border=ALB_BRD)

# 2. Erase MatchMaker Lambda ───────────────────────────────────────────────
erase(draw, ERASE_MATCHMK)
service_box(draw,
    x1=264, y1=182, x2=350, y2=258,
    line1="ECS Fargate", line2="FastAPI", line3="(Match + Rank)",
    label_below="ECS Fargate",
    bg=ECS_BG, border=ECS_BRD)

# 3. Erase GetRank Lambda ──────────────────────────────────────────────────
erase(draw, ERASE_GETRANK)
service_box(draw,
    x1=30,  y1=396, x2=122, y2=462,
    line1="ECS Fargate", line2="(same task)", line3="",
    label_below="ECS Fargate",
    bg=ECS_BG, border=ECS_BRD)

# 4. Note below GetRank ECS box indicating it is the same Fargate service
#    Placed below the label text, well clear of any other components
NOTE_CX = 76
NOTE_Y  = 496
# small pill background so the note stands out
draw.rounded_rectangle([(NOTE_CX - 62, NOTE_Y - 10),
                         (NOTE_CX + 62, NOTE_Y + 10)],
                        radius=5, fill=(220, 235, 250, 255),
                        outline=ECS_BRD, width=1)
draw.text((NOTE_CX, NOTE_Y), "= same Fargate task",
          fill=ECS_BG, anchor="mm", font=F_SMALL)

img_b = add_banner(img_b, "Scenario B — ECS Fargate + ALB", BANNER_B)
img_b.save(OUT_B, dpi=(160, 160))
print(f"  Saved {OUT_B}")
print("Done.")
