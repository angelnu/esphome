# Commands collected

## Known commands:

- 0x01: Sound active (sent by door)
- 0x02: Start sound. Same as 0x39? Video?
- 0x04: 
- 0x05: 
- 0x09: Ring Bell
- 0x0C: Open Door
- 0x21: Stop sound
- 0x28: 
- 0x2C: Light
- 0x39: Start sound
- 

## Known addresses:

- 08:00: Door Light?
- 08:02: Door
- 08:02: Flat XX
- 08:04: Flat XX
- 08:06: Flat XX
- 08:08: Small flat
- 08:0E: Flat XX
- 08:1E: Large flat

## Unknown messages

### Received

- 02:0D:
  - 08:03:12 - Command: 0x04, Destination: 02:0D, Source: 08:0C
- 12:0D:
  - 08:03:13 - Command: 0x04, Destination: 12:0D, Source: 08:0C
- 1E:0D:
  - 08:03:23 - Command: 0x28, Destination: 1E:0D, Source: 08:0C
- 0C:0D
  - 19:22:54.052 - Command: 04, Destination: 0C:0D, Source: 08:02
    - Unexpected middle 06
    - Unexpected epilog 02
  - 19:22:54.052 - Command: 04, Destination: 0C:0D, Source: 08:02
    - Unexpected middle 06
    - Unexpected epilog 02
  - 19:22:54.996 - Command: 04, Destination: 0C:0D, Source: 08:12
    - Unexpected middle 06
    - Unexpected epilog 02
  - 19:23:05.482 - Command: 28, Destination: 0C:0D, Source: 08:1E
    - Unexpected middle 06
    - Unexpected epilog 02

### Sent

- 08:02
  - 19:22:54.052 - Command: 04, Destination: 0C:0D, Source: 08:02
    - Unexpected middle 06
    - Unexpected epilog 02
  - 19:22:54.052 - Command: 04, Destination: 0C:0D, Source: 08:02
    - Unexpected middle 06
    - Unexpected epilog 02
- 08:06
  - 16:25:13 - Command: 05, Destination: 08:06, Source: 08:06
  - 12:26:15 - Command: 05, Destination: 08:06, Source: 08:06
  - 12:26:17 - Command: 05, Destination: 08:06, Source: 08:06
  - 12:26:19 - Command: 05, Destination: 08:06, Source: 08:06
- 08:12
  - 19:22:54.996 - Command: 04, Destination: 0C:0D, Source: 08:12
    - Unexpected middle 06
    - Unexpected epilog 02
- 08:0C
  - 08:03:12 - Command: 0x04, Destination: 02:0D, Source: 08:0C
  - 08:03:13 - Command: 0x04, Destination: 12:0D, Source: 08:0C
  - 08:03:23 - Command: 0x28, Destination: 1E:0D, Source: 08:0C
- 08:1E (large flat)
  - 19:23:05.482 - Command: 28, Destination: 0C:0D, Source: 08:1E
    - Unexpected middle 06
    - Unexpected epilog 02

## Messages

