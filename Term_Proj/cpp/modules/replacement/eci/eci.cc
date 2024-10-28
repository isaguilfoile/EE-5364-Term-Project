/* Achieving Non-Inclusive Cache Performance with Inclusive Caches: Temporal Locality Aware (TLA) Cache Management Policies
   (https://ieeexplore.ieee.org/document/5695533) */

// Early Core Invalidation

/**
 * TODO: Early Core Invalidation (ECI) derives the temporal locality of a line
 * before it becomes LRU in the LLC. The LLC choses a line early and invalidates
 * that line in the core caches, while leaving the line in the LLC. By observing
 * the core's subsequent request, the LLC derives the temporal locality.
 *
 *
 * See (Section B)
 */