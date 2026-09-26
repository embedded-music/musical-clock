set dotenv-load := true

default:
    @just --list

test:
    pio test -e native --project-dir packages/musical-clock

check: test
