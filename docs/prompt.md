❯ write me a prompt that makes an ai agent like claude create a markdown file in ./docs/ that includes a full and complete developer guide and documentation of the DNS protocol. make it explain every aspect of the protocol and its usage conceptually and on the byte level. It is too supposed to act as way to program a dns server by oneself

Here's a prompt you can paste into Claude Code (or another agent). It's written for your easydns repo, so it asks for C++ examples and tells the agent to look at your existing code first.

---

# Task: Write a complete DNS protocol developer guide

Create one Markdown file at `./docs/dns-protocol-guide.md`. It should be a complete, self-contained developer guide to the DNS protocol. A competent programmer who has never worked with DNS should be able to read it and then write a working DNS server from scratch (authoritative first, then a recursive resolver) without opening any other source.

## Before you write

1. Look through the repository (especially `include/` and `src/`) to see what already exists, for example the DNS header implementation and the networking code. Match code examples to the project's language (C++) and naming style. Where the existing code is relevant, point to it, but don't let it limit what the guide covers.
2. Use the RFCs as the source of truth: RFC 1034 and 1035 (the core), plus 1123, 2181, 2308, 3596, 2782, 6891, 6895, 7766, 8020, 8482, 8499, 4033–4035, 5155, 7873, 9250, 8484 and 7858. Cite the RFC and section for every normative claim, like `(RFC 1035 §4.1.1)`. If you can't verify something, say so. Never make up field values, codes or behavior.

## Requirements for every section

- **Conceptual layer:** what it is, why it exists, and how it fits into the whole system.
- **Byte layer:** exact wire format, with bit and byte offsets, field sizes, endianness (network byte order) and allowed values. Use ASCII diagrams in the RFC style:
  ```
                                  1  1  1  1  1  1
    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  |                      ID                       |
  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  ```
- **Worked hex examples:** real annotated hex dumps of queries and responses, decoded field by field. Every byte must be accounted for.
- **Implementation notes:** how to parse and serialize it, what to validate, common bugs, security pitfalls, and edge cases.
- **Code:** short C++ snippets for the important parsing and serialization routines.

## Required contents (at minimum)

1. **Introduction and mental model:** the namespace tree, zones and delegation, authoritative vs. recursive vs. stub vs. forwarder, caching, and the full lifecycle of a lookup for `www.example.com` starting from the root.
2. **Terminology glossary** (aligned with RFC 8499).
3. **Domain names:** labels, length limits (63/255), allowed characters, case-insensitivity and case preservation, the root label, presentation format vs. wire format, escaping (`\.`, `\DDD`), and IDNA/punycode basics.
4. **Message format:** overall structure (Header, Question, Answer, Authority, Additional).
   - The header in full detail: ID, QR, OPCODE, AA, TC, RD, RA, Z, AD, CD, RCODE, and the four counts, with bitmasks and shift operations for packing and unpacking flags.
   - The question section: QNAME, QTYPE, QCLASS.
   - The resource record format: NAME, TYPE, CLASS, TTL, RDLENGTH, RDATA.
5. **Name compression:** pointer format (`0xC0` prefix, 14-bit offset), when you may and may not compress, how to write a compression table when encoding, and safe decoding (loop detection, bounds checks, forward pointers, maximum jump count).
6. **Record types:** for each type, give the purpose, the RDATA wire layout, the presentation format and a hex example. Cover A, AAAA, NS, CNAME, SOA, PTR, MX, TXT, SRV, CAA, NAPTR, DNAME, HINFO, OPT, ANY/`*`, AXFR/IXFR, and the DNSSEC types (DNSKEY, RRSIG, DS, NSEC, NSEC3, NSEC3PARAM). Also cover SVCB/HTTPS. Include a table of TYPE and CLASS codes.
7. **Opcodes and RCODEs:** all standard values, including extended RCODEs through EDNS, and what a server must send in each situation.
8. **Transport:** UDP on port 53 and the 512-byte limit, truncation (TC) and falling back to TCP, the TCP 2-byte length prefix (RFC 7766), connection reuse and pipelining, timeouts, and message size limits. Also give a short overview of DoT, DoH and DoQ.
9. **EDNS(0):** the OPT pseudo-RR layout (how the CLASS and TTL fields are repurposed), UDP payload size, the DO bit, options (ECS, Cookies, Padding, Extended DNS Errors), and how to handle version mismatches and BADVERS.
10. **Zones and zone files:** master file syntax (RFC 1035 §5), `$ORIGIN`, `$TTL`, `$INCLUDE`, `@`, relative names, parentheses, comments, SOA semantics (serial arithmetic per RFC 1982, refresh, retry, expire, minimum), and glue records.
11. **Authoritative server logic:** the complete lookup algorithm from RFC 1034 §4.3.2, step by step in pseudocode. Cover finding the closest enclosing zone, exact match, CNAME chasing, DNAME, delegation and referrals (with glue in the additional section), wildcards (RFC 4592), NXDOMAIN vs. NODATA, when to set AA, and how to fill the additional section.
12. **Recursive resolver logic:** iterative resolution starting from root hints, following referrals, using glue and bailiwick checks, chasing CNAMEs, QNAME minimisation, and selecting and retrying servers.
13. **Caching:** TTL handling, negative caching (RFC 2308), how to represent RRsets, TTL capping, and serve-stale.
14. **Zone transfers and updates:** AXFR, IXFR, NOTIFY, Dynamic UPDATE (RFC 2136), and a TSIG overview.
15. **DNSSEC:** the chain of trust, how signing and validation work, the RRSIG/DNSKEY/DS byte layouts, canonical ordering and canonical form, authenticated denial with NSEC and NSEC3, and the AD and CD bits. Include a validation algorithm in pseudocode.
16. **Security:** cache poisoning and the Kaminsky attack, ID and source-port randomization, 0x20 case randomization, amplification and reflection with response rate limiting, malformed packet handling, compression bombs, and a checklist of input validation you must do.
17. **Building a DNS server step by step:** a hands-on tutorial with these stages:
    1. A UDP socket that receives and echoes packets.
    2. Parse the header and question.
    3. Answer a hard-coded A record.
    4. Load records from a zone file.
    5. Name compression in responses.
    6. A full authoritative lookup (referrals, CNAMEs, NXDOMAIN, wildcards).
    7. TCP support and truncation.
    8. EDNS(0).
    9. A recursive resolver with a cache.
    10. Hardening.

    Give each stage its goal, the code and data structures it needs, and how to test it.
18. **Testing and debugging:** using `dig` (with `+norec`, `+tcp`, `+dnssec`, `+bufsize`, and `@127.0.0.1 -p 5353`), `drill`, `kdig`, Wireshark/tcpdump filters, comparing against BIND or Unbound, and building a test-vector set out of captured packets.
19. **Appendices:** complete tables of TYPE, CLASS, OPCODE, RCODE, EDNS option codes and DNSSEC algorithm numbers. Add a list of RFCs, each with a one-line summary, a parsing and serialization checklist, and an FAQ of common mistakes.

## Format

- Put a clickable table of contents at the top, and use numbered headings.
- Use fenced code blocks with language tags (`cpp`, `text`, `bash`).
- Use tables for field definitions and code lists.
- Be exhaustive. Length is fine. Precision and completeness matter more than brevity. Where something is out of scope (for example, the full math of DNSSEC crypto algorithms), say so explicitly and link to the RFC.
- Write it in English, directly and technically, with no filler.

## Before you finish

Re-read the whole document and check the following:
- Every hex example adds up byte for byte, and the counts and lengths match.
- Every field offset and bitmask is correct.
- Every section listed above is present.
- There are no placeholders or TODOs.

Then give a short summary of what the file contains.

---

Tips:

- Length: A guide this size may hit output limits in a single pass. If it gets cut off, add: "Write the file section by section, appending each one, and confirm each section is complete before moving on."
- More accuracy: If the agent has web access, add "Fetch the RFC texts from rfc-editor.org and verify against them" to cut down on wrong details.
- Another language: If you want a different language, change "C++" in the "Before you write" section and in the requirements list. Take your time.