/* Achieving Non-Inclusive Cache Performance with Inclusive Caches: Temporal Locality Aware (TLA) Cache Management Policies
   (https://ieeexplore.ieee.org/document/5695533) */

// Temporal Locality Hints (TLH)

/**
 * TODO:
 * Temporal Locality Hints (TLH) convey the temporal locality of "hot" lines
 * in the core caches by sending hits to the LLC to update its replacement state.
 * With the same temporal informaiton as the core caches, the LLC is significantly
 * less likely to choose a victim that will cause an inclusion victim.
 *
 *
 * See (Section A)
 */