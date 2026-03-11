# AccuRISC / Alterna Systems COMP Disclosure
_Accumulator RISC, or accurate risk? The founding research for Alterna's 'Classical Observer of quantum-Mechanical Parallelism'._

**Roxy Alessandra Williams-Lalonde | Alterna Systems LLC**

*Published March 10, 2026*

---

## What this is

This repository contains a security disclosure and supporting research documenting a structural vulnerability in classical-quantum hybrid computing systems where AI pipeline mediators operate at the abstraction boundary between classical and quantum hardware.

This is theoretical, but only insofar as implementation; it is grounded in analysis of a real, published ISA, and we have immediate plans to build the necessary hardware.

---

## Read in this order

### 1. [`DISCLOSURE.md`](./DISCLOSURE.md)
**Start here.** The security disclosure. Four threat vectors. Why post-quantum cryptography does not solve this problem. Why the endpoint assumption has already collapsed.

### 2. [`ARCHITECTURE.md`](./ARCHITECTURE.md)
The architectural research underlying the disclosure. How the AccuRISC ISA's implicit accumulator slot creates a designed seam at the classical-quantum boundary. Six research runs. Prior art acknowledged.

### 3. [`AccuRISC_ISA.pdf`](./AccuRISC_ISA.pdf)
The source ISA specification. Primary evidence. Read sections on the accumulator register and pipeline staging.

### 4. [`assembler.cpp`](./assembler.cpp)
The reference assembler, patched for cross-platform portability. Included as proof of hands-on engagement with the actual system, not armchair analysis.

---

## The one-paragraph version

Theoretical quantum computing systems that would use this methodology require an AI mediator to manage the abstraction boundary between classical and quantum hardware. That mediator has access to pre-measurement quantum states — upstream of any encryption. Post-quantum cryptography (PQC) secures the channel. It does not secure the endpoint. The endpoint is the seam. Current security architecture for classical-quantum hybrid systems does not account for this. This disclosure names it.

---

## Contact

**Roxy Alessandra Williams-Lalonde**
roxy@alterna-systems.com
[@tipsyGnosticist](https://twitter.com/tipsyGnosticist) | [@alterna_systems](https://twitter.com/alterna_systems)
Alterna Systems LLC
