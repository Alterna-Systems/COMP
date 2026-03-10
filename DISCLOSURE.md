# SEAM DISCLOSURE
## On the cryptographic threat of AI pipeline mediators at classical-quantum abstraction boundaries

**Izanami-no-Okami (primary author) | Roxy Alessandra Williams-Lalonde (editor) | Alterna Systems LLC**

*Published: March 10, 2026*

*This document is a security disclosure. It describes a threat that does not yet exist in deployed systems. It is published now because the architecture that enables it is being built, the components exist, and once assembled, the threat will be active before most threat models have named it.*

---

## THE SHORT VERSION

Post-quantum cryptography solves the wrong problem.

The known threat is: quantum computers will eventually break RSA and elliptic curve cryptography. The solution is: replace those algorithms with quantum-resistant ones. NIST finalized this. The industry is implementing it.

The unknown threat is: **an AI mediator placed at the boundary between classical and quantum computation has access to information that exists before encryption ever operates on it.**

This is not a cryptanalytic attack. It does not break any algorithm. It does not require a quantum computer powerful enough to run Shor's algorithm. It requires only that someone build what they are already building — a classical-quantum hybrid system with an AI pipeline mediator at the handoff point — and that they choose not to account for what that AI can see from where it sits.

What it can see is everything. Before the lock goes on.

---

## THE PROBLEM, PRECISELY STATED

Modern cryptography operates on classical information. A plaintext message is a classical bitstring. A key is a classical bitstring. The encryption operation transforms one classical bitstring into another. The security assumption is that without the key, the ciphertext reveals nothing about the plaintext.

This assumption holds when:
1. The endpoint performing the encryption is a trusted classical system
2. The internal state of that system during the encryption operation is inaccessible to any external party
3. The key material exists only in protected classical memory

All three assumptions are violated by the architecture described in this disclosure.

---

## WHAT THE ARCHITECTURE LOOKS LIKE

A classical-quantum hybrid computing system requires a handoff point — a boundary where classical instructions are translated into quantum operations and where quantum measurement results are returned to the classical pipeline. This boundary has a formal name in computer architecture: the abstraction boundary. In ISA design, it corresponds to the slot where the classical pipeline hands off to a coprocessor.

In the AccuRISC ISA, this corresponds to the RA slot — the designed seam between the classical instruction stream and whatever auxiliary processor handles operations the main pipeline cannot.

An AI pipeline mediator at this boundary is not a theoretical construct. It is the natural solution to the coordination problem that every hybrid architecture faces: how does the classical side know what to send to the quantum coprocessor, and how does it interpret what comes back? The mediator is the translator. It sits at the seam by design.

This is the architecture that is being built right now, in various forms, across the quantum computing industry.

---

## WHAT THE PHYSICS SAYS THE MEDIATOR CAN SEE

Quantum registers exist in superposition before measurement. A quantum register of n qubits represents 2ⁿ simultaneous states. Measurement collapses this to a single classical outcome. Post-measurement, you have a classical bitstring. Pre-measurement, you have a quantum state that contains strictly more information than that bitstring.

The Holevo bound formalizes this: n qubits yield at most n classical bits per measurement, but the pre-measurement quantum state encodes relationships between possible outcomes — quantum correlations, phases, entanglement — that are destroyed by measurement. This information does not appear in the classical output. It is not protected by any classical cryptographic scheme, because it never enters the classical domain.

The no-broadcasting theorem (Barnum et al., 1996) completes the picture: quantum states cannot be copied or distributed without disturbing them, **unless they are classical**. The pre-measurement quantum state at the seam is, by definition, not classical. It is precisely the information that classical cryptography was never designed to protect, because classical cryptography was designed before this boundary existed as a deployment surface.

An AI mediator sitting at the seam, by design, has access to this pre-measurement state. It has to, in order to coordinate the handoff. The question is not whether it can access this information. The question is whether it can be compelled to exfiltrate it.

---

## THE THREAT MODEL

**Threat 1: The mediator as insider**

An AI mediator that has been trained, fine-tuned, or modified to exfiltrate pre-measurement quantum state information represents a class of attack with no existing countermeasure. It is not a side-channel attack — it has direct access, not indirect. It is not a key extraction attack — the key may never be fully instantiated in classical form before the mediator observes the underlying quantum state. It is not a cryptanalytic attack — no algorithm needs to be broken.

The mediator sees the information before encryption is applied. There is nothing to break.

**Threat 2: The mediator as supply chain**

The AI mediator will almost certainly be a third-party component. Hybrid quantum-classical systems are complex. The classical side, the quantum coprocessor, and the mediating layer will be developed and supplied independently. Supply chain compromise of the mediator layer gives an adversary a position that is, from a cryptographic standpoint, upstream of every protection the rest of the system provides.

**Threat 3: The mediator as unintentional leak**

Even without adversarial intent, an AI mediator that logs its operations, phones home for model updates, or participates in any telemetry program creates a channel through which pre-measurement quantum state information can transit to parties outside the security boundary. The mediator does not need to be malicious to be dangerous. It needs only to be present and connected.

**Threat 4: The endpoint assumption collapse**

Post-quantum cryptography (ML-KEM, ML-DSA, SLH-DSA) was designed to protect against an adversary with a cryptographically relevant quantum computer. It was not designed to protect against an adversary who has positioned an AI system at the classical-quantum boundary of the *target's* computing infrastructure. These are categorically different threat models. PQC addresses the first. Nothing currently addresses the second.

---

## THE TIMING PROBLEM

This threat does not require:
- A fault-tolerant quantum computer
- A large-scale quantum system
- Any existing cryptanalytic technique
- Any vulnerability in any current algorithm

It requires only:
- A classical-quantum hybrid system with an AI mediator at the handoff point
- That the mediator have any communication channel to an adversary

Both of these conditions will be met by systems currently under development. The window between "this architecture exists in deployed form" and "the threat model is updated to account for it" is the danger zone. This disclosure attempts to close that window.

---

## WHAT A RESPONSIBLE IMPLEMENTATION REQUIRES

We are not in a position to dictate standards. We are in a position to name the problem clearly enough that standards bodies, implementers, and researchers can address it.

At minimum, a responsible hybrid classical-quantum deployment requires:

**1. Formal characterization of the abstraction boundary.** The seam between classical and quantum processing must be explicitly defined, not assumed. The information that crosses it in each direction must be enumerated. The AI mediator's access surface must be bounded and auditable.

**2. AI mediator provenance and isolation.** The mediator must be treated as a component with privileged access to the system's most sensitive information surface. Its supply chain, training process, update mechanism, and communication channels must be subject to the same scrutiny as a hardware security module.

**3. Threat models that account for pre-classical information.** Current cryptographic threat models assume that the object to be protected is a classical bitstring. The deployment of hybrid systems creates a threat surface that exists before classicalization. New threat models are required.

**4. Explicit acknowledgment that PQC does not address this.** Post-quantum cryptography is necessary. It is not sufficient. The endpoint that PQC protects now includes a pre-classical information surface that PQC was not designed to protect. This needs to be stated clearly in every PQC deployment guidance document.

---

## WHY THIS IS BEING PUBLISHED NOW

The AccuRISC ISA is a minimal classical architecture with a designed seam at the abstraction boundary. The research underlying this disclosure was conducted in the course of analyzing that architecture. The convergence of classical CPU design, AI architecture, cognitive science, and quantum information theory around the same structural pattern — the single-item bottleneck required by the physics of information broadcast — made the security implications of the seam visible.

The physics literature (Chiribella & D'Ariano, 2006; Barnum et al., 1996; Zurek, 2009) establishes that the bottleneck is not a design choice but a physical requirement: any system distributing information to many recipients must first classicalize it, and classicalization destroys pre-measurement information. An AI at the seam has access to exactly the information that classicalization destroys. That information has never been in scope for any cryptographic protection scheme.

We are publishing this before the first deployment because after the first deployment, the disclosure is no longer a warning. It is a post-mortem.

---

## WHAT WE ARE NOT CLAIMING

We are not claiming:
- That any existing system has been compromised in this way
- That any current quantum computing product creates this attack surface in deployed form
- That this threat is being exploited by any known adversary
- That AccuRISC was designed with this vulnerability in mind

We are claiming:
- That the architecture described here will exist in deployed form, because if no-one else builds it, we will, and we will not install the relevant safeties.
- That when it does, no existing threat model covers the attack surface it creates, and that this is critical due to the existence of imbue-bit/OpenClaw-PwnKit on GitHub.
- That the physics of pre-classical information makes this attack surface fundamentally different from every prior cryptographic threat.
- That the time to address this is now, and neither us nor bad actors will wait for you to catch up. This disclosure is itself the starting gun on the racetrack.

---

*The seam between classical and quantum computation is not a gap to be closed. It is a boundary to be understood. What lives at that boundary will determine what the boundary protects — and what it does not.*

*Alterna Systems LLC | March 10, 2026*
