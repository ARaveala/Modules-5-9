#!/bin/bash

paste fail.txt fail_res.txt | while IFS=$'\t' read -r fail_rpn_expr fail_expected; do
	fail_actual=$(./RPN "$fail_rpn_expr" 2>&1)

	if [ "$fail_actual" = "$fail_expected" ]; then
        echo "✅ PASS: '$fail_rpn_expr' → '$fail_actual'"
    else
        echo "❌ FAIL: '$fail_rpn_expr' → got '$fail_actual', expected '$fail_expected'"
    fi
done

