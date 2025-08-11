#!/bin/bash

paste success.txt success_res.txt | while IFS=$'\t' read -r rpn_expr expected; do
	actual=$(./RPN "$rpn_expr")
	if [ "$actual" = "$expected" ]; then
        echo "✅ PASS: '$rpn_expr' → '$actual'"
    else
        echo "❌ FAIL: '$rpn_expr' → got '$actual', expected '$expected'"
    fi
done

