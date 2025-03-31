/**
 * @brief Taping / Wengert List (Most Common Optimization):

    Concept: Instead of building a persistent graph structure with nodes pointing to each other, you perform a forward pass (evaluate) and record a linear sequence (the "tape" or Wengert list) of the elementary operations performed and the values involved.

    Implementation: You'd typically have a global (or thread-local) "tape" object (often a std::vector of operation records). When you overload operator+, sin(), etc., they don't create a heap node; instead, they:

        Evaluate their inputs (which might recursively add to the tape).

        Perform the calculation (e.g., a + b).

        Push an "operation record" onto the tape. This record stores the operation type (e.g., ADD, SIN), indices pointing to the operands on the tape, and the result value.

        Return a lightweight handle or index referring to the result on the tape.

    Benefits:

        Reduced Heap Allocation: Only the tape itself (usually a vector, which grows dynamically but more efficiently than many small allocations) uses significant heap storage. Handles/indices are often small.

        Eliminates Recursion for Backward Pass: The crucial reverse pass becomes an iterative process. You iterate backwards through the linear tape. For each operation record, you use the stored values and the already-computed derivative of the output of that operation to compute the derivatives of its inputs, accumulating them using the chain rule. This is much faster and avoids stack overflow issues.

        Cache Locality: Processing a linear tape can often have better cache performance than chasing pointers in a potentially fragmented graph structure.
 * 
 */
namespace AD {

};  // namespace AD
