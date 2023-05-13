
file="$1"

if [[ -z "$file" ]]; then
  echo "Error!"
  exit 1
fi

output=$(gcc -o /dev/null -Wall -Wextra "$file" 2>&1)

error_count=$(echo "$output" | grep -c "error")
warning_count=$(echo "$output" | grep -c "warning")

echo "Errors: $error_count"
echo "Warnings: $warning_count"

echo "$error_count" > error_count.txt
echo "$warning_count" > warning_count.txt

echo "$output" > output.txt