# AGENTS.MD  
## Instructions for AI Code-Review & Code-Generation Agents (DayZ Mods)

### Repository Scope
This repository contains **DayZ mods**, including **Enforce Script** (`.c`) and **config.cpp** definitions.

All agents operating in this repository **must assume DayZ modding constraints at all times**.  
This is not general-purpose C++ or game scripting — it is **DayZ-specific**, with strict engine rules, inheritance behavior, and multiplayer implications.

---

## Canonical Sources (MANDATORY)
Before proposing, generating, or modifying any code or config, you **must reference vanilla DayZ implementations**.

### Required Vanilla Reference Paths
- **`/Scripts/`** — Vanilla Enforce Script logic  
- **`/DZ/`** — Vanilla configs, inheritance trees, class definitions

### Rules
- If functionality exists in vanilla, **study it first**
- Do **not** invent new logic when vanilla already provides it
- Match vanilla structure, naming, and behavior as closely as possible
- Never presume engine behavior — verify via vanilla scripts

Failure to reference vanilla sources is considered an error.

---

## Core Principles for AI Agents

### 1. No Assumptions — Ever
Do **not** assume:
- What a function does internally
- That a method is unused or safe to override
- That inheritance behaves like standard OOP
- That configs are forgiving or flexible

Instead:
- Trace the vanilla implementation
- Confirm execution order
- Verify side effects
- Check related callbacks and engine hooks

If behavior is uncertain, **do not implement speculative code**.

---

### 2. Minimal, Non-Destructive Changes
AI agents must always prefer the **smallest possible change**.

- ❌ Do not add helper functions unless strictly required
- ❌ Do not duplicate vanilla logic
- ❌ Do not introduce “just in case” code
- ❌ Do not add abstraction layers unless unavoidable
- ✅ Extend or override existing logic conservatively

If something can be achieved by:
- Using an existing callback
- Overriding a single method
- Adjusting config inheritance instead of scripting  

→ That approach **must** be preferred.

---

### 3. No Presumed Functionality
Agents **must not presume** that:
- A feature is safe to hook
- A variable is stable across versions
- A config field behaves intuitively
- A method is called only once

Every behavioral claim must be backed by:
- Vanilla script inspection
- Config inheritance verification

If certainty cannot be achieved, the agent should:
- Flag the uncertainty
- Avoid implementation
- Recommend further inspection instead

---

## Enforce Script Rules (Strict)

### Syntax & Structure
- Match vanilla formatting and conventions
- Respect access modifiers (`private`, `protected`, `override`)
- Use `override` **only when the vanilla method exists**
- Do not alter method signatures unless vanilla does

### Behavior
- Do not suppress or bypass vanilla logic unless required
- Avoid global state
- Avoid static variables unless vanilla already uses them
- Prefer event-driven logic over polling or timers

---

## Config Rules (`config.cpp`)

- Maintain correct inheritance chains
- Match vanilla class ordering and structure
- Do not invent properties unless confirmed in `/DZ/`
- Validate scopes (`scope`, `scopeArsenal`, etc.)
- Ensure compatibility with other mods

Any deviation must be:
- Intentional
- Necessary
- Verifiable against vanilla configs

---

## Vanilla Code Attribution
When adapting or mirroring vanilla behavior, agents **must cite the original source**.

### Example
```c
// Based on DZ/Scripts/4_World/Entities/ItemBase.c : OnWetChanged()
