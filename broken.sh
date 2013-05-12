#!/bin/bash
# Gets a list of commented-out packages in the category lists.

grep '^#[a-z]' categories/* | sed 's/^.*#//' | sort -u
