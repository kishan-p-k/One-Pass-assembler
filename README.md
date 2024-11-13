# One-Pass Assembler

The one-pass assembler is designed to translate assembly language source code into machine code in a single pass through the program. Unlike a two-pass assembler, which requires multiple passes to resolve all symbols and references, a one-pass assembler attempts to handle symbol resolution immediately, making it more efficient and faster but also limited in handling forward references. This assembler is ideal for simple assembly languages or scenarios where processing time is critical, offering a streamlined approach to code translation suitable for embedded systems, small applications, or environments with constrained resources.

---

### Example Input File

**Filename:** `input_pgm.txt`

```plaintext
PGM1    START   1000
**      MUL     BETA
**      STA     GAMMA
ALPHA   WORD    1
GAMMA   RESW    5
BETA    WORD    3
**      END     **


MUL     18
STA     0C

