#!/bin/bash

cd categories
grep -v '^#' * |
grep -v '^all'
