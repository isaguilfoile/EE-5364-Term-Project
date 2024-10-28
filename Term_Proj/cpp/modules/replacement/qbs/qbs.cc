/* Achieving Non-Inclusive Cache Performance with Inclusive Caches: Temporal Locality Aware (TLA) Cache Management Policies
   (https://ieeexplore.ieee.org/document/5695533) */

// Query Based Selection (QBS)

/**
 * TODO: Query Based Selection (QBS) infers the temporal locality of a line
 * in the LLC by querying the core caches. The LLC selects a line for replacement
 * and queries the core caches for approval. The LLC uses the information provided
 * by the core caches to infer the temporal locality and select a victim. QBS
 * employs the heuristic that the core has better information about temporal locality,
 * and it is best suited to propose which lines can be evicted
 *
 *
 * See (Section C)
 */