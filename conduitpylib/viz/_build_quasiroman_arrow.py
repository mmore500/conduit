def build_quasiroman_arrow(n: int) -> str:
    # Simple Roman numeral conversion using only I, V, and X
    roman_parts = []
    num = n
    roman_parts.extend(["M"] * (num // 1000))  # Add 'X' for each multiple of 10
    num %= 1000
    roman_parts.extend(["D"] * (num // 500))  # Add 'X' for each multiple of 10
    num %= 500
    roman_parts.extend(["C"] * (num // 100))  # Add 'X' for each multiple of 10
    num %= 100
    roman_parts.extend(["L"] * (num // 50))  # Add 'X' for each multiple of 10
    num %= 50
    roman_parts.extend(["X"] * (num // 10))  # Add 'X' for each multiple of 10
    num %= 10
    roman_parts.extend(["V"] * (num // 5))  # Add 'V' for each multiple of 5
    num %= 5
    roman_parts.extend([r"/"] * num)  # Add 'I' for the remainder

    # Join with " \! " to form the arrow-like structure
    res = "$" + r" \! ".join(roman_parts) + r" \! $-$\!\!\guilsinglright$"
    print(n, res)
    return res
