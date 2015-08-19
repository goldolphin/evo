/**
 * 
 * @author goldolphin
 *         2015-05-02 12:01
 */
#ifndef EVO_OPERATORS_H
#define EVO_OPERATORS_H


// Precedence	Operator	Associativity	Description
// 1	++ --	Left-to-right	// Suffix/postfix increment and decrement
// 1	()	Left-to-right	// Function call
// 1	[]	Left-to-right	// Array subscripting
// 1	.	Left-to-right	// Structure and union member access
// 1	−>	Left-to-right	// Structure and union member access through pointer
// 1	(type){list}	Left-to-right	// Compound literal(C99)
// 2	++ --	Right-to-left	// Prefix increment and decrement
// 2	+ −	Right-to-left	// Unary plus and minus
// 2	! ~	Right-to-left	// Logical NOT and bitwise NOT
// 2	(type)	Right-to-left	// Type cast
// 2	*	Right-to-left	// Indirection (dereference)
// 2	&	Right-to-left	// Address-of
// 2	sizeof	Right-to-left	// Size-of
// 2	_Alignof	Right-to-left	// Alignment requirement(C11)
// 3	* / %	Left-to-right	// Multiplication, division, and remainder
// 4	+ −	Left-to-right	// Addition and subtraction
// 5	<< >>	Left-to-right	// Bitwise left shift and right shift
// 6	< <=	Left-to-right	// For relational operators < and ≤ respectively
// 6	> >=	Left-to-right	// For relational operators > and ≥ respectively
// 7	== !=	Left-to-right	// For relational = and ≠ respectively
// 8	&	Left-to-right	// Bitwise AND
// 9	^	Left-to-right	// Bitwise XOR (exclusive or)
// 10	|	Left-to-right	// Bitwise OR (inclusive or)
// 11	&&	Left-to-right	// Logical AND
// 12	||	Left-to-right	// Logical OR
// 13	=	Right-to-left	// Simple assignment

#endif //EVO_OPERATORS_H
