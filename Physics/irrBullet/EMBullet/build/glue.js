
// Bindings utilities

function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc: function(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy: function(array, view, offset) {
    var offsetShifted = offset;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offsetShifted >>= 1; break;
      case 4: offsetShifted >>= 2; break;
      case 8: offsetShifted >>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offsetShifted + i] = array[i];
    }
  },
};

function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// btCollisionShape
/** @suppress {undefinedVars, duplicate} */function btCollisionShape() { throw "cannot construct a btCollisionShape, no constructor in IDL" }
btCollisionShape.prototype = Object.create(WrapperObject.prototype);
btCollisionShape.prototype.constructor = btCollisionShape;
btCollisionShape.prototype.__class__ = btCollisionShape;
btCollisionShape.__cache__ = {};
Module['btCollisionShape'] = btCollisionShape;

btCollisionShape.prototype['setLocalScaling'] = btCollisionShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCollisionShape_setLocalScaling_1(self, scaling);
};;

btCollisionShape.prototype['calculateLocalInertia'] = btCollisionShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCollisionShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btCollisionShape.prototype['__destroy__'] = btCollisionShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCollisionShape___destroy___0(self);
};
// btCollisionObject
/** @suppress {undefinedVars, duplicate} */function btCollisionObject() { throw "cannot construct a btCollisionObject, no constructor in IDL" }
btCollisionObject.prototype = Object.create(WrapperObject.prototype);
btCollisionObject.prototype.constructor = btCollisionObject;
btCollisionObject.prototype.__class__ = btCollisionObject;
btCollisionObject.__cache__ = {};
Module['btCollisionObject'] = btCollisionObject;

btCollisionObject.prototype['setAnisotropicFriction'] = btCollisionObject.prototype.setAnisotropicFriction = /** @suppress {undefinedVars, duplicate} */function(anisotropicFriction, frictionMode) {
  var self = this.ptr;
  if (anisotropicFriction && typeof anisotropicFriction === 'object') anisotropicFriction = anisotropicFriction.ptr;
  if (frictionMode && typeof frictionMode === 'object') frictionMode = frictionMode.ptr;
  _emscripten_bind_btCollisionObject_setAnisotropicFriction_2(self, anisotropicFriction, frictionMode);
};;

btCollisionObject.prototype['getCollisionShape'] = btCollisionObject.prototype.getCollisionShape = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionObject_getCollisionShape_0(self), btCollisionShape);
};;

btCollisionObject.prototype['setContactProcessingThreshold'] = btCollisionObject.prototype.setContactProcessingThreshold = /** @suppress {undefinedVars, duplicate} */function(contactProcessingThreshold) {
  var self = this.ptr;
  if (contactProcessingThreshold && typeof contactProcessingThreshold === 'object') contactProcessingThreshold = contactProcessingThreshold.ptr;
  _emscripten_bind_btCollisionObject_setContactProcessingThreshold_1(self, contactProcessingThreshold);
};;

btCollisionObject.prototype['setActivationState'] = btCollisionObject.prototype.setActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btCollisionObject_setActivationState_1(self, newState);
};;

btCollisionObject.prototype['forceActivationState'] = btCollisionObject.prototype.forceActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btCollisionObject_forceActivationState_1(self, newState);
};;

btCollisionObject.prototype['activate'] = btCollisionObject.prototype.activate = /** @suppress {undefinedVars, duplicate} */function(forceActivation) {
  var self = this.ptr;
  if (forceActivation && typeof forceActivation === 'object') forceActivation = forceActivation.ptr;
  if (forceActivation === undefined) { _emscripten_bind_btCollisionObject_activate_0(self);  return }
  _emscripten_bind_btCollisionObject_activate_1(self, forceActivation);
};;

btCollisionObject.prototype['isActive'] = btCollisionObject.prototype.isActive = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btCollisionObject_isActive_0(self));
};;

btCollisionObject.prototype['isKinematicObject'] = btCollisionObject.prototype.isKinematicObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btCollisionObject_isKinematicObject_0(self));
};;

btCollisionObject.prototype['setRestitution'] = btCollisionObject.prototype.setRestitution = /** @suppress {undefinedVars, duplicate} */function(rest) {
  var self = this.ptr;
  if (rest && typeof rest === 'object') rest = rest.ptr;
  _emscripten_bind_btCollisionObject_setRestitution_1(self, rest);
};;

btCollisionObject.prototype['setFriction'] = btCollisionObject.prototype.setFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btCollisionObject_setFriction_1(self, frict);
};;

btCollisionObject.prototype['setRollingFriction'] = btCollisionObject.prototype.setRollingFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btCollisionObject_setRollingFriction_1(self, frict);
};;

btCollisionObject.prototype['getWorldTransform'] = btCollisionObject.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionObject_getWorldTransform_0(self), btTransform);
};;

btCollisionObject.prototype['getCollisionFlags'] = btCollisionObject.prototype.getCollisionFlags = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btCollisionObject_getCollisionFlags_0(self);
};;

btCollisionObject.prototype['setCollisionFlags'] = btCollisionObject.prototype.setCollisionFlags = /** @suppress {undefinedVars, duplicate} */function(flags) {
  var self = this.ptr;
  if (flags && typeof flags === 'object') flags = flags.ptr;
  _emscripten_bind_btCollisionObject_setCollisionFlags_1(self, flags);
};;

btCollisionObject.prototype['setWorldTransform'] = btCollisionObject.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btCollisionObject_setWorldTransform_1(self, worldTrans);
};;

btCollisionObject.prototype['setCollisionShape'] = btCollisionObject.prototype.setCollisionShape = /** @suppress {undefinedVars, duplicate} */function(collisionShape) {
  var self = this.ptr;
  if (collisionShape && typeof collisionShape === 'object') collisionShape = collisionShape.ptr;
  _emscripten_bind_btCollisionObject_setCollisionShape_1(self, collisionShape);
};;

btCollisionObject.prototype['setCcdMotionThreshold'] = btCollisionObject.prototype.setCcdMotionThreshold = /** @suppress {undefinedVars, duplicate} */function(ccdMotionThreshold) {
  var self = this.ptr;
  if (ccdMotionThreshold && typeof ccdMotionThreshold === 'object') ccdMotionThreshold = ccdMotionThreshold.ptr;
  _emscripten_bind_btCollisionObject_setCcdMotionThreshold_1(self, ccdMotionThreshold);
};;

btCollisionObject.prototype['setCcdSweptSphereRadius'] = btCollisionObject.prototype.setCcdSweptSphereRadius = /** @suppress {undefinedVars, duplicate} */function(radius) {
  var self = this.ptr;
  if (radius && typeof radius === 'object') radius = radius.ptr;
  _emscripten_bind_btCollisionObject_setCcdSweptSphereRadius_1(self, radius);
};;

  btCollisionObject.prototype['__destroy__'] = btCollisionObject.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCollisionObject___destroy___0(self);
};
// btConcaveShape
/** @suppress {undefinedVars, duplicate} */function btConcaveShape() { throw "cannot construct a btConcaveShape, no constructor in IDL" }
btConcaveShape.prototype = Object.create(btCollisionShape.prototype);
btConcaveShape.prototype.constructor = btConcaveShape;
btConcaveShape.prototype.__class__ = btConcaveShape;
btConcaveShape.__cache__ = {};
Module['btConcaveShape'] = btConcaveShape;

btConcaveShape.prototype['setLocalScaling'] = btConcaveShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConcaveShape_setLocalScaling_1(self, scaling);
};;

btConcaveShape.prototype['calculateLocalInertia'] = btConcaveShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConcaveShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btConcaveShape.prototype['__destroy__'] = btConcaveShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConcaveShape___destroy___0(self);
};
// btCollisionWorld
/** @suppress {undefinedVars, duplicate} */function btCollisionWorld() { throw "cannot construct a btCollisionWorld, no constructor in IDL" }
btCollisionWorld.prototype = Object.create(WrapperObject.prototype);
btCollisionWorld.prototype.constructor = btCollisionWorld;
btCollisionWorld.prototype.__class__ = btCollisionWorld;
btCollisionWorld.__cache__ = {};
Module['btCollisionWorld'] = btCollisionWorld;

btCollisionWorld.prototype['getDispatcher'] = btCollisionWorld.prototype.getDispatcher = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionWorld_getDispatcher_0(self), btDispatcher);
};;

btCollisionWorld.prototype['rayTest'] = btCollisionWorld.prototype.rayTest = /** @suppress {undefinedVars, duplicate} */function(rayFromWorld, rayToWorld, resultCallback) {
  var self = this.ptr;
  if (rayFromWorld && typeof rayFromWorld === 'object') rayFromWorld = rayFromWorld.ptr;
  if (rayToWorld && typeof rayToWorld === 'object') rayToWorld = rayToWorld.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btCollisionWorld_rayTest_3(self, rayFromWorld, rayToWorld, resultCallback);
};;

btCollisionWorld.prototype['getPairCache'] = btCollisionWorld.prototype.getPairCache = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionWorld_getPairCache_0(self), btOverlappingPairCache);
};;

btCollisionWorld.prototype['getDispatchInfo'] = btCollisionWorld.prototype.getDispatchInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionWorld_getDispatchInfo_0(self), btDispatcherInfo);
};;

btCollisionWorld.prototype['addCollisionObject'] = btCollisionWorld.prototype.addCollisionObject = /** @suppress {undefinedVars, duplicate} */function(collisionObject, collisionFilterGroup, collisionFilterMask) {
  var self = this.ptr;
  if (collisionObject && typeof collisionObject === 'object') collisionObject = collisionObject.ptr;
  if (collisionFilterGroup && typeof collisionFilterGroup === 'object') collisionFilterGroup = collisionFilterGroup.ptr;
  if (collisionFilterMask && typeof collisionFilterMask === 'object') collisionFilterMask = collisionFilterMask.ptr;
  if (collisionFilterGroup === undefined) { _emscripten_bind_btCollisionWorld_addCollisionObject_1(self, collisionObject);  return }
  if (collisionFilterMask === undefined) { _emscripten_bind_btCollisionWorld_addCollisionObject_2(self, collisionObject, collisionFilterGroup);  return }
  _emscripten_bind_btCollisionWorld_addCollisionObject_3(self, collisionObject, collisionFilterGroup, collisionFilterMask);
};;

btCollisionWorld.prototype['getBroadphase'] = btCollisionWorld.prototype.getBroadphase = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btCollisionWorld_getBroadphase_0(self), btBroadphaseInterface);
};;

btCollisionWorld.prototype['convexSweepTest'] = btCollisionWorld.prototype.convexSweepTest = /** @suppress {undefinedVars, duplicate} */function(castShape, from, to, resultCallback, allowedCcdPenetration) {
  var self = this.ptr;
  if (castShape && typeof castShape === 'object') castShape = castShape.ptr;
  if (from && typeof from === 'object') from = from.ptr;
  if (to && typeof to === 'object') to = to.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  if (allowedCcdPenetration && typeof allowedCcdPenetration === 'object') allowedCcdPenetration = allowedCcdPenetration.ptr;
  _emscripten_bind_btCollisionWorld_convexSweepTest_5(self, castShape, from, to, resultCallback, allowedCcdPenetration);
};;

btCollisionWorld.prototype['contactPairTest'] = btCollisionWorld.prototype.contactPairTest = /** @suppress {undefinedVars, duplicate} */function(colObjA, colObjB, resultCallback) {
  var self = this.ptr;
  if (colObjA && typeof colObjA === 'object') colObjA = colObjA.ptr;
  if (colObjB && typeof colObjB === 'object') colObjB = colObjB.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btCollisionWorld_contactPairTest_3(self, colObjA, colObjB, resultCallback);
};;

  btCollisionWorld.prototype['__destroy__'] = btCollisionWorld.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCollisionWorld___destroy___0(self);
};
// btTypedConstraint
/** @suppress {undefinedVars, duplicate} */function btTypedConstraint() { throw "cannot construct a btTypedConstraint, no constructor in IDL" }
btTypedConstraint.prototype = Object.create(WrapperObject.prototype);
btTypedConstraint.prototype.constructor = btTypedConstraint;
btTypedConstraint.prototype.__class__ = btTypedConstraint;
btTypedConstraint.__cache__ = {};
Module['btTypedConstraint'] = btTypedConstraint;

btTypedConstraint.prototype['enableFeedback'] = btTypedConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btTypedConstraint_enableFeedback_1(self, needsFeedback);
};;

btTypedConstraint.prototype['getBreakingImpulseThreshold'] = btTypedConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btTypedConstraint_getBreakingImpulseThreshold_0(self);
};;

btTypedConstraint.prototype['setBreakingImpulseThreshold'] = btTypedConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btTypedConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btTypedConstraint.prototype['__destroy__'] = btTypedConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btTypedConstraint___destroy___0(self);
};
// btCapsuleShape
/** @suppress {undefinedVars, duplicate} */function btCapsuleShape(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btCapsuleShape_btCapsuleShape_2(radius, height);
  getCache(btCapsuleShape)[this.ptr] = this;
};;
btCapsuleShape.prototype = Object.create(btCollisionShape.prototype);
btCapsuleShape.prototype.constructor = btCapsuleShape;
btCapsuleShape.prototype.__class__ = btCapsuleShape;
btCapsuleShape.__cache__ = {};
Module['btCapsuleShape'] = btCapsuleShape;

btCapsuleShape.prototype['setLocalScaling'] = btCapsuleShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCapsuleShape_setLocalScaling_1(self, scaling);
};;

btCapsuleShape.prototype['calculateLocalInertia'] = btCapsuleShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCapsuleShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btCapsuleShape.prototype['__destroy__'] = btCapsuleShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCapsuleShape___destroy___0(self);
};
// btGeneric6DofConstraint
/** @suppress {undefinedVars, duplicate} */function btGeneric6DofConstraint(rbA, rbB, frameInA, frameInB, useLinearFrameReferenceFrameA) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (frameInA && typeof frameInA === 'object') frameInA = frameInA.ptr;
  if (frameInB && typeof frameInB === 'object') frameInB = frameInB.ptr;
  if (useLinearFrameReferenceFrameA && typeof useLinearFrameReferenceFrameA === 'object') useLinearFrameReferenceFrameA = useLinearFrameReferenceFrameA.ptr;
  if (frameInB === undefined) { this.ptr = _emscripten_bind_btGeneric6DofConstraint_btGeneric6DofConstraint_3(rbA, rbB, frameInA); getCache(btGeneric6DofConstraint)[this.ptr] = this;return }
  if (useLinearFrameReferenceFrameA === undefined) { this.ptr = _emscripten_bind_btGeneric6DofConstraint_btGeneric6DofConstraint_4(rbA, rbB, frameInA, frameInB); getCache(btGeneric6DofConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btGeneric6DofConstraint_btGeneric6DofConstraint_5(rbA, rbB, frameInA, frameInB, useLinearFrameReferenceFrameA);
  getCache(btGeneric6DofConstraint)[this.ptr] = this;
};;
btGeneric6DofConstraint.prototype = Object.create(btTypedConstraint.prototype);
btGeneric6DofConstraint.prototype.constructor = btGeneric6DofConstraint;
btGeneric6DofConstraint.prototype.__class__ = btGeneric6DofConstraint;
btGeneric6DofConstraint.__cache__ = {};
Module['btGeneric6DofConstraint'] = btGeneric6DofConstraint;

btGeneric6DofConstraint.prototype['setLinearLowerLimit'] = btGeneric6DofConstraint.prototype.setLinearLowerLimit = /** @suppress {undefinedVars, duplicate} */function(linearLower) {
  var self = this.ptr;
  if (linearLower && typeof linearLower === 'object') linearLower = linearLower.ptr;
  _emscripten_bind_btGeneric6DofConstraint_setLinearLowerLimit_1(self, linearLower);
};;

btGeneric6DofConstraint.prototype['setLinearUpperLimit'] = btGeneric6DofConstraint.prototype.setLinearUpperLimit = /** @suppress {undefinedVars, duplicate} */function(linearUpper) {
  var self = this.ptr;
  if (linearUpper && typeof linearUpper === 'object') linearUpper = linearUpper.ptr;
  _emscripten_bind_btGeneric6DofConstraint_setLinearUpperLimit_1(self, linearUpper);
};;

btGeneric6DofConstraint.prototype['setAngularLowerLimit'] = btGeneric6DofConstraint.prototype.setAngularLowerLimit = /** @suppress {undefinedVars, duplicate} */function(angularLower) {
  var self = this.ptr;
  if (angularLower && typeof angularLower === 'object') angularLower = angularLower.ptr;
  _emscripten_bind_btGeneric6DofConstraint_setAngularLowerLimit_1(self, angularLower);
};;

btGeneric6DofConstraint.prototype['setAngularUpperLimit'] = btGeneric6DofConstraint.prototype.setAngularUpperLimit = /** @suppress {undefinedVars, duplicate} */function(angularUpper) {
  var self = this.ptr;
  if (angularUpper && typeof angularUpper === 'object') angularUpper = angularUpper.ptr;
  _emscripten_bind_btGeneric6DofConstraint_setAngularUpperLimit_1(self, angularUpper);
};;

btGeneric6DofConstraint.prototype['enableFeedback'] = btGeneric6DofConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btGeneric6DofConstraint_enableFeedback_1(self, needsFeedback);
};;

btGeneric6DofConstraint.prototype['getBreakingImpulseThreshold'] = btGeneric6DofConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btGeneric6DofConstraint_getBreakingImpulseThreshold_0(self);
};;

btGeneric6DofConstraint.prototype['setBreakingImpulseThreshold'] = btGeneric6DofConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btGeneric6DofConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btGeneric6DofConstraint.prototype['__destroy__'] = btGeneric6DofConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btGeneric6DofConstraint___destroy___0(self);
};
// btStridingMeshInterface
/** @suppress {undefinedVars, duplicate} */function btStridingMeshInterface() { throw "cannot construct a btStridingMeshInterface, no constructor in IDL" }
btStridingMeshInterface.prototype = Object.create(WrapperObject.prototype);
btStridingMeshInterface.prototype.constructor = btStridingMeshInterface;
btStridingMeshInterface.prototype.__class__ = btStridingMeshInterface;
btStridingMeshInterface.__cache__ = {};
Module['btStridingMeshInterface'] = btStridingMeshInterface;

  btStridingMeshInterface.prototype['__destroy__'] = btStridingMeshInterface.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btStridingMeshInterface___destroy___0(self);
};
// ConvexResultCallback
/** @suppress {undefinedVars, duplicate} */function ConvexResultCallback() { throw "cannot construct a ConvexResultCallback, no constructor in IDL" }
ConvexResultCallback.prototype = Object.create(WrapperObject.prototype);
ConvexResultCallback.prototype.constructor = ConvexResultCallback;
ConvexResultCallback.prototype.__class__ = ConvexResultCallback;
ConvexResultCallback.__cache__ = {};
Module['ConvexResultCallback'] = ConvexResultCallback;

ConvexResultCallback.prototype['hasHit'] = ConvexResultCallback.prototype.hasHit = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_ConvexResultCallback_hasHit_0(self));
};;

  ConvexResultCallback.prototype['get_m_collisionFilterGroup'] = ConvexResultCallback.prototype.get_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ConvexResultCallback_get_m_collisionFilterGroup_0(self);
};
    ConvexResultCallback.prototype['set_m_collisionFilterGroup'] = ConvexResultCallback.prototype.set_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ConvexResultCallback_set_m_collisionFilterGroup_1(self, arg0);
};
    Object.defineProperty(ConvexResultCallback.prototype, 'm_collisionFilterGroup', { get: ConvexResultCallback.prototype.get_m_collisionFilterGroup, set: ConvexResultCallback.prototype.set_m_collisionFilterGroup });
  ConvexResultCallback.prototype['get_m_collisionFilterMask'] = ConvexResultCallback.prototype.get_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ConvexResultCallback_get_m_collisionFilterMask_0(self);
};
    ConvexResultCallback.prototype['set_m_collisionFilterMask'] = ConvexResultCallback.prototype.set_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ConvexResultCallback_set_m_collisionFilterMask_1(self, arg0);
};
    Object.defineProperty(ConvexResultCallback.prototype, 'm_collisionFilterMask', { get: ConvexResultCallback.prototype.get_m_collisionFilterMask, set: ConvexResultCallback.prototype.set_m_collisionFilterMask });
  ConvexResultCallback.prototype['get_m_closestHitFraction'] = ConvexResultCallback.prototype.get_m_closestHitFraction = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ConvexResultCallback_get_m_closestHitFraction_0(self);
};
    ConvexResultCallback.prototype['set_m_closestHitFraction'] = ConvexResultCallback.prototype.set_m_closestHitFraction = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ConvexResultCallback_set_m_closestHitFraction_1(self, arg0);
};
    Object.defineProperty(ConvexResultCallback.prototype, 'm_closestHitFraction', { get: ConvexResultCallback.prototype.get_m_closestHitFraction, set: ConvexResultCallback.prototype.set_m_closestHitFraction });
  ConvexResultCallback.prototype['__destroy__'] = ConvexResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_ConvexResultCallback___destroy___0(self);
};
// btMotionState
/** @suppress {undefinedVars, duplicate} */function btMotionState() { throw "cannot construct a btMotionState, no constructor in IDL" }
btMotionState.prototype = Object.create(WrapperObject.prototype);
btMotionState.prototype.constructor = btMotionState;
btMotionState.prototype.__class__ = btMotionState;
btMotionState.__cache__ = {};
Module['btMotionState'] = btMotionState;

btMotionState.prototype['getWorldTransform'] = btMotionState.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btMotionState_getWorldTransform_1(self, worldTrans);
};;

btMotionState.prototype['setWorldTransform'] = btMotionState.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btMotionState_setWorldTransform_1(self, worldTrans);
};;

  btMotionState.prototype['__destroy__'] = btMotionState.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btMotionState___destroy___0(self);
};
// btConeShape
/** @suppress {undefinedVars, duplicate} */function btConeShape(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btConeShape_btConeShape_2(radius, height);
  getCache(btConeShape)[this.ptr] = this;
};;
btConeShape.prototype = Object.create(btCollisionShape.prototype);
btConeShape.prototype.constructor = btConeShape;
btConeShape.prototype.__class__ = btConeShape;
btConeShape.__cache__ = {};
Module['btConeShape'] = btConeShape;

btConeShape.prototype['setLocalScaling'] = btConeShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConeShape_setLocalScaling_1(self, scaling);
};;

btConeShape.prototype['calculateLocalInertia'] = btConeShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConeShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btConeShape.prototype['__destroy__'] = btConeShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConeShape___destroy___0(self);
};
// btConvexShape
/** @suppress {undefinedVars, duplicate} */function btConvexShape() { throw "cannot construct a btConvexShape, no constructor in IDL" }
btConvexShape.prototype = Object.create(btCollisionShape.prototype);
btConvexShape.prototype.constructor = btConvexShape;
btConvexShape.prototype.__class__ = btConvexShape;
btConvexShape.__cache__ = {};
Module['btConvexShape'] = btConvexShape;

btConvexShape.prototype['setLocalScaling'] = btConvexShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConvexShape_setLocalScaling_1(self, scaling);
};;

btConvexShape.prototype['calculateLocalInertia'] = btConvexShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConvexShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btConvexShape.prototype['__destroy__'] = btConvexShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConvexShape___destroy___0(self);
};
// ContactResultCallback
/** @suppress {undefinedVars, duplicate} */function ContactResultCallback() { throw "cannot construct a ContactResultCallback, no constructor in IDL" }
ContactResultCallback.prototype = Object.create(WrapperObject.prototype);
ContactResultCallback.prototype.constructor = ContactResultCallback;
ContactResultCallback.prototype.__class__ = ContactResultCallback;
ContactResultCallback.__cache__ = {};
Module['ContactResultCallback'] = ContactResultCallback;

ContactResultCallback.prototype['addSingleResult'] = ContactResultCallback.prototype.addSingleResult = /** @suppress {undefinedVars, duplicate} */function(cp, colObj0Wrap, partId0, index0, colObj1Wrap, partId1, index1) {
  var self = this.ptr;
  if (cp && typeof cp === 'object') cp = cp.ptr;
  if (colObj0Wrap && typeof colObj0Wrap === 'object') colObj0Wrap = colObj0Wrap.ptr;
  if (partId0 && typeof partId0 === 'object') partId0 = partId0.ptr;
  if (index0 && typeof index0 === 'object') index0 = index0.ptr;
  if (colObj1Wrap && typeof colObj1Wrap === 'object') colObj1Wrap = colObj1Wrap.ptr;
  if (partId1 && typeof partId1 === 'object') partId1 = partId1.ptr;
  if (index1 && typeof index1 === 'object') index1 = index1.ptr;
  return _emscripten_bind_ContactResultCallback_addSingleResult_7(self, cp, colObj0Wrap, partId0, index0, colObj1Wrap, partId1, index1);
};;

  ContactResultCallback.prototype['__destroy__'] = ContactResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_ContactResultCallback___destroy___0(self);
};
// btTriangleMeshShape
/** @suppress {undefinedVars, duplicate} */function btTriangleMeshShape() { throw "cannot construct a btTriangleMeshShape, no constructor in IDL" }
btTriangleMeshShape.prototype = Object.create(btConcaveShape.prototype);
btTriangleMeshShape.prototype.constructor = btTriangleMeshShape;
btTriangleMeshShape.prototype.__class__ = btTriangleMeshShape;
btTriangleMeshShape.__cache__ = {};
Module['btTriangleMeshShape'] = btTriangleMeshShape;

btTriangleMeshShape.prototype['setLocalScaling'] = btTriangleMeshShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btTriangleMeshShape_setLocalScaling_1(self, scaling);
};;

btTriangleMeshShape.prototype['calculateLocalInertia'] = btTriangleMeshShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btTriangleMeshShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btTriangleMeshShape.prototype['__destroy__'] = btTriangleMeshShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btTriangleMeshShape___destroy___0(self);
};
// btDispatcher
/** @suppress {undefinedVars, duplicate} */function btDispatcher() { throw "cannot construct a btDispatcher, no constructor in IDL" }
btDispatcher.prototype = Object.create(WrapperObject.prototype);
btDispatcher.prototype.constructor = btDispatcher;
btDispatcher.prototype.__class__ = btDispatcher;
btDispatcher.__cache__ = {};
Module['btDispatcher'] = btDispatcher;

btDispatcher.prototype['getNumManifolds'] = btDispatcher.prototype.getNumManifolds = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcher_getNumManifolds_0(self);
};;

btDispatcher.prototype['getManifoldByIndexInternal'] = btDispatcher.prototype.getManifoldByIndexInternal = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btDispatcher_getManifoldByIndexInternal_1(self, index), btPersistentManifold);
};;

  btDispatcher.prototype['__destroy__'] = btDispatcher.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDispatcher___destroy___0(self);
};
// btDynamicsWorld
/** @suppress {undefinedVars, duplicate} */function btDynamicsWorld() { throw "cannot construct a btDynamicsWorld, no constructor in IDL" }
btDynamicsWorld.prototype = Object.create(btCollisionWorld.prototype);
btDynamicsWorld.prototype.constructor = btDynamicsWorld;
btDynamicsWorld.prototype.__class__ = btDynamicsWorld;
btDynamicsWorld.__cache__ = {};
Module['btDynamicsWorld'] = btDynamicsWorld;

btDynamicsWorld.prototype['addAction'] = btDynamicsWorld.prototype.addAction = /** @suppress {undefinedVars, duplicate} */function(action) {
  var self = this.ptr;
  if (action && typeof action === 'object') action = action.ptr;
  _emscripten_bind_btDynamicsWorld_addAction_1(self, action);
};;

btDynamicsWorld.prototype['getSolverInfo'] = btDynamicsWorld.prototype.getSolverInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDynamicsWorld_getSolverInfo_0(self), btContactSolverInfo);
};;

btDynamicsWorld.prototype['getDispatcher'] = btDynamicsWorld.prototype.getDispatcher = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDynamicsWorld_getDispatcher_0(self), btDispatcher);
};;

btDynamicsWorld.prototype['rayTest'] = btDynamicsWorld.prototype.rayTest = /** @suppress {undefinedVars, duplicate} */function(rayFromWorld, rayToWorld, resultCallback) {
  var self = this.ptr;
  if (rayFromWorld && typeof rayFromWorld === 'object') rayFromWorld = rayFromWorld.ptr;
  if (rayToWorld && typeof rayToWorld === 'object') rayToWorld = rayToWorld.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btDynamicsWorld_rayTest_3(self, rayFromWorld, rayToWorld, resultCallback);
};;

btDynamicsWorld.prototype['getPairCache'] = btDynamicsWorld.prototype.getPairCache = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDynamicsWorld_getPairCache_0(self), btOverlappingPairCache);
};;

btDynamicsWorld.prototype['getDispatchInfo'] = btDynamicsWorld.prototype.getDispatchInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDynamicsWorld_getDispatchInfo_0(self), btDispatcherInfo);
};;

btDynamicsWorld.prototype['addCollisionObject'] = btDynamicsWorld.prototype.addCollisionObject = /** @suppress {undefinedVars, duplicate} */function(collisionObject, collisionFilterGroup, collisionFilterMask) {
  var self = this.ptr;
  if (collisionObject && typeof collisionObject === 'object') collisionObject = collisionObject.ptr;
  if (collisionFilterGroup && typeof collisionFilterGroup === 'object') collisionFilterGroup = collisionFilterGroup.ptr;
  if (collisionFilterMask && typeof collisionFilterMask === 'object') collisionFilterMask = collisionFilterMask.ptr;
  if (collisionFilterGroup === undefined) { _emscripten_bind_btDynamicsWorld_addCollisionObject_1(self, collisionObject);  return }
  if (collisionFilterMask === undefined) { _emscripten_bind_btDynamicsWorld_addCollisionObject_2(self, collisionObject, collisionFilterGroup);  return }
  _emscripten_bind_btDynamicsWorld_addCollisionObject_3(self, collisionObject, collisionFilterGroup, collisionFilterMask);
};;

btDynamicsWorld.prototype['getBroadphase'] = btDynamicsWorld.prototype.getBroadphase = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDynamicsWorld_getBroadphase_0(self), btBroadphaseInterface);
};;

btDynamicsWorld.prototype['convexSweepTest'] = btDynamicsWorld.prototype.convexSweepTest = /** @suppress {undefinedVars, duplicate} */function(castShape, from, to, resultCallback, allowedCcdPenetration) {
  var self = this.ptr;
  if (castShape && typeof castShape === 'object') castShape = castShape.ptr;
  if (from && typeof from === 'object') from = from.ptr;
  if (to && typeof to === 'object') to = to.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  if (allowedCcdPenetration && typeof allowedCcdPenetration === 'object') allowedCcdPenetration = allowedCcdPenetration.ptr;
  _emscripten_bind_btDynamicsWorld_convexSweepTest_5(self, castShape, from, to, resultCallback, allowedCcdPenetration);
};;

btDynamicsWorld.prototype['contactPairTest'] = btDynamicsWorld.prototype.contactPairTest = /** @suppress {undefinedVars, duplicate} */function(colObjA, colObjB, resultCallback) {
  var self = this.ptr;
  if (colObjA && typeof colObjA === 'object') colObjA = colObjA.ptr;
  if (colObjB && typeof colObjB === 'object') colObjB = colObjB.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btDynamicsWorld_contactPairTest_3(self, colObjA, colObjB, resultCallback);
};;

  btDynamicsWorld.prototype['__destroy__'] = btDynamicsWorld.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDynamicsWorld___destroy___0(self);
};
// RayResultCallback
/** @suppress {undefinedVars, duplicate} */function RayResultCallback() { throw "cannot construct a RayResultCallback, no constructor in IDL" }
RayResultCallback.prototype = Object.create(WrapperObject.prototype);
RayResultCallback.prototype.constructor = RayResultCallback;
RayResultCallback.prototype.__class__ = RayResultCallback;
RayResultCallback.__cache__ = {};
Module['RayResultCallback'] = RayResultCallback;

RayResultCallback.prototype['hasHit'] = RayResultCallback.prototype.hasHit = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_RayResultCallback_hasHit_0(self));
};;

  RayResultCallback.prototype['get_m_collisionFilterGroup'] = RayResultCallback.prototype.get_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_RayResultCallback_get_m_collisionFilterGroup_0(self);
};
    RayResultCallback.prototype['set_m_collisionFilterGroup'] = RayResultCallback.prototype.set_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_RayResultCallback_set_m_collisionFilterGroup_1(self, arg0);
};
    Object.defineProperty(RayResultCallback.prototype, 'm_collisionFilterGroup', { get: RayResultCallback.prototype.get_m_collisionFilterGroup, set: RayResultCallback.prototype.set_m_collisionFilterGroup });
  RayResultCallback.prototype['get_m_collisionFilterMask'] = RayResultCallback.prototype.get_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_RayResultCallback_get_m_collisionFilterMask_0(self);
};
    RayResultCallback.prototype['set_m_collisionFilterMask'] = RayResultCallback.prototype.set_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_RayResultCallback_set_m_collisionFilterMask_1(self, arg0);
};
    Object.defineProperty(RayResultCallback.prototype, 'm_collisionFilterMask', { get: RayResultCallback.prototype.get_m_collisionFilterMask, set: RayResultCallback.prototype.set_m_collisionFilterMask });
  RayResultCallback.prototype['get_m_collisionObject'] = RayResultCallback.prototype.get_m_collisionObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_RayResultCallback_get_m_collisionObject_0(self), btCollisionObject);
};
    RayResultCallback.prototype['set_m_collisionObject'] = RayResultCallback.prototype.set_m_collisionObject = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_RayResultCallback_set_m_collisionObject_1(self, arg0);
};
    Object.defineProperty(RayResultCallback.prototype, 'm_collisionObject', { get: RayResultCallback.prototype.get_m_collisionObject, set: RayResultCallback.prototype.set_m_collisionObject });
  RayResultCallback.prototype['__destroy__'] = RayResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_RayResultCallback___destroy___0(self);
};
// btVector3
/** @suppress {undefinedVars, duplicate} */function btVector3(x, y, z) {
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  if (x === undefined) { this.ptr = _emscripten_bind_btVector3_btVector3_0(); getCache(btVector3)[this.ptr] = this;return }
  if (y === undefined) { this.ptr = _emscripten_bind_btVector3_btVector3_1(x); getCache(btVector3)[this.ptr] = this;return }
  if (z === undefined) { this.ptr = _emscripten_bind_btVector3_btVector3_2(x, y); getCache(btVector3)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btVector3_btVector3_3(x, y, z);
  getCache(btVector3)[this.ptr] = this;
};;
btVector3.prototype = Object.create(WrapperObject.prototype);
btVector3.prototype.constructor = btVector3;
btVector3.prototype.__class__ = btVector3;
btVector3.__cache__ = {};
Module['btVector3'] = btVector3;

btVector3.prototype['length'] = btVector3.prototype.length = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector3_length_0(self);
};;

btVector3.prototype['x'] = btVector3.prototype.x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector3_x_0(self);
};;

btVector3.prototype['y'] = btVector3.prototype.y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector3_y_0(self);
};;

btVector3.prototype['z'] = btVector3.prototype.z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector3_z_0(self);
};;

btVector3.prototype['setX'] = btVector3.prototype.setX = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_btVector3_setX_1(self, x);
};;

btVector3.prototype['setY'] = btVector3.prototype.setY = /** @suppress {undefinedVars, duplicate} */function(y) {
  var self = this.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  _emscripten_bind_btVector3_setY_1(self, y);
};;

btVector3.prototype['setZ'] = btVector3.prototype.setZ = /** @suppress {undefinedVars, duplicate} */function(z) {
  var self = this.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_btVector3_setZ_1(self, z);
};;

btVector3.prototype['setValue'] = btVector3.prototype.setValue = /** @suppress {undefinedVars, duplicate} */function(x, y, z) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_btVector3_setValue_3(self, x, y, z);
};;

btVector3.prototype['op_mul'] = btVector3.prototype.op_mul = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  return wrapPointer(_emscripten_bind_btVector3_op_mul_1(self, x), btVector3);
};;

  btVector3.prototype['__destroy__'] = btVector3.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btVector3___destroy___0(self);
};
// btVehicleRaycaster
/** @suppress {undefinedVars, duplicate} */function btVehicleRaycaster() { throw "cannot construct a btVehicleRaycaster, no constructor in IDL" }
btVehicleRaycaster.prototype = Object.create(WrapperObject.prototype);
btVehicleRaycaster.prototype.constructor = btVehicleRaycaster;
btVehicleRaycaster.prototype.__class__ = btVehicleRaycaster;
btVehicleRaycaster.__cache__ = {};
Module['btVehicleRaycaster'] = btVehicleRaycaster;

  btVehicleRaycaster.prototype['__destroy__'] = btVehicleRaycaster.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btVehicleRaycaster___destroy___0(self);
};
// btGhostObject
/** @suppress {undefinedVars, duplicate} */function btGhostObject() {
  this.ptr = _emscripten_bind_btGhostObject_btGhostObject_0();
  getCache(btGhostObject)[this.ptr] = this;
};;
btGhostObject.prototype = Object.create(btCollisionObject.prototype);
btGhostObject.prototype.constructor = btGhostObject;
btGhostObject.prototype.__class__ = btGhostObject;
btGhostObject.__cache__ = {};
Module['btGhostObject'] = btGhostObject;

btGhostObject.prototype['getNumOverlappingObjects'] = btGhostObject.prototype.getNumOverlappingObjects = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btGhostObject_getNumOverlappingObjects_0(self);
};;

btGhostObject.prototype['getOverlappingObject'] = btGhostObject.prototype.getOverlappingObject = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btGhostObject_getOverlappingObject_1(self, index), btCollisionObject);
};;

btGhostObject.prototype['setAnisotropicFriction'] = btGhostObject.prototype.setAnisotropicFriction = /** @suppress {undefinedVars, duplicate} */function(anisotropicFriction, frictionMode) {
  var self = this.ptr;
  if (anisotropicFriction && typeof anisotropicFriction === 'object') anisotropicFriction = anisotropicFriction.ptr;
  if (frictionMode && typeof frictionMode === 'object') frictionMode = frictionMode.ptr;
  _emscripten_bind_btGhostObject_setAnisotropicFriction_2(self, anisotropicFriction, frictionMode);
};;

btGhostObject.prototype['getCollisionShape'] = btGhostObject.prototype.getCollisionShape = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btGhostObject_getCollisionShape_0(self), btCollisionShape);
};;

btGhostObject.prototype['setContactProcessingThreshold'] = btGhostObject.prototype.setContactProcessingThreshold = /** @suppress {undefinedVars, duplicate} */function(contactProcessingThreshold) {
  var self = this.ptr;
  if (contactProcessingThreshold && typeof contactProcessingThreshold === 'object') contactProcessingThreshold = contactProcessingThreshold.ptr;
  _emscripten_bind_btGhostObject_setContactProcessingThreshold_1(self, contactProcessingThreshold);
};;

btGhostObject.prototype['setActivationState'] = btGhostObject.prototype.setActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btGhostObject_setActivationState_1(self, newState);
};;

btGhostObject.prototype['forceActivationState'] = btGhostObject.prototype.forceActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btGhostObject_forceActivationState_1(self, newState);
};;

btGhostObject.prototype['activate'] = btGhostObject.prototype.activate = /** @suppress {undefinedVars, duplicate} */function(forceActivation) {
  var self = this.ptr;
  if (forceActivation && typeof forceActivation === 'object') forceActivation = forceActivation.ptr;
  if (forceActivation === undefined) { _emscripten_bind_btGhostObject_activate_0(self);  return }
  _emscripten_bind_btGhostObject_activate_1(self, forceActivation);
};;

btGhostObject.prototype['isActive'] = btGhostObject.prototype.isActive = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btGhostObject_isActive_0(self));
};;

btGhostObject.prototype['isKinematicObject'] = btGhostObject.prototype.isKinematicObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btGhostObject_isKinematicObject_0(self));
};;

btGhostObject.prototype['setRestitution'] = btGhostObject.prototype.setRestitution = /** @suppress {undefinedVars, duplicate} */function(rest) {
  var self = this.ptr;
  if (rest && typeof rest === 'object') rest = rest.ptr;
  _emscripten_bind_btGhostObject_setRestitution_1(self, rest);
};;

btGhostObject.prototype['setFriction'] = btGhostObject.prototype.setFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btGhostObject_setFriction_1(self, frict);
};;

btGhostObject.prototype['setRollingFriction'] = btGhostObject.prototype.setRollingFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btGhostObject_setRollingFriction_1(self, frict);
};;

btGhostObject.prototype['getWorldTransform'] = btGhostObject.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btGhostObject_getWorldTransform_0(self), btTransform);
};;

btGhostObject.prototype['getCollisionFlags'] = btGhostObject.prototype.getCollisionFlags = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btGhostObject_getCollisionFlags_0(self);
};;

btGhostObject.prototype['setCollisionFlags'] = btGhostObject.prototype.setCollisionFlags = /** @suppress {undefinedVars, duplicate} */function(flags) {
  var self = this.ptr;
  if (flags && typeof flags === 'object') flags = flags.ptr;
  _emscripten_bind_btGhostObject_setCollisionFlags_1(self, flags);
};;

btGhostObject.prototype['setWorldTransform'] = btGhostObject.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btGhostObject_setWorldTransform_1(self, worldTrans);
};;

btGhostObject.prototype['setCollisionShape'] = btGhostObject.prototype.setCollisionShape = /** @suppress {undefinedVars, duplicate} */function(collisionShape) {
  var self = this.ptr;
  if (collisionShape && typeof collisionShape === 'object') collisionShape = collisionShape.ptr;
  _emscripten_bind_btGhostObject_setCollisionShape_1(self, collisionShape);
};;

btGhostObject.prototype['setCcdMotionThreshold'] = btGhostObject.prototype.setCcdMotionThreshold = /** @suppress {undefinedVars, duplicate} */function(ccdMotionThreshold) {
  var self = this.ptr;
  if (ccdMotionThreshold && typeof ccdMotionThreshold === 'object') ccdMotionThreshold = ccdMotionThreshold.ptr;
  _emscripten_bind_btGhostObject_setCcdMotionThreshold_1(self, ccdMotionThreshold);
};;

btGhostObject.prototype['setCcdSweptSphereRadius'] = btGhostObject.prototype.setCcdSweptSphereRadius = /** @suppress {undefinedVars, duplicate} */function(radius) {
  var self = this.ptr;
  if (radius && typeof radius === 'object') radius = radius.ptr;
  _emscripten_bind_btGhostObject_setCcdSweptSphereRadius_1(self, radius);
};;

  btGhostObject.prototype['__destroy__'] = btGhostObject.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btGhostObject___destroy___0(self);
};
// btQuadWord
/** @suppress {undefinedVars, duplicate} */function btQuadWord() { throw "cannot construct a btQuadWord, no constructor in IDL" }
btQuadWord.prototype = Object.create(WrapperObject.prototype);
btQuadWord.prototype.constructor = btQuadWord;
btQuadWord.prototype.__class__ = btQuadWord;
btQuadWord.__cache__ = {};
Module['btQuadWord'] = btQuadWord;

btQuadWord.prototype['x'] = btQuadWord.prototype.x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuadWord_x_0(self);
};;

btQuadWord.prototype['y'] = btQuadWord.prototype.y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuadWord_y_0(self);
};;

btQuadWord.prototype['z'] = btQuadWord.prototype.z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuadWord_z_0(self);
};;

btQuadWord.prototype['w'] = btQuadWord.prototype.w = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuadWord_w_0(self);
};;

btQuadWord.prototype['setX'] = btQuadWord.prototype.setX = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_btQuadWord_setX_1(self, x);
};;

btQuadWord.prototype['setY'] = btQuadWord.prototype.setY = /** @suppress {undefinedVars, duplicate} */function(y) {
  var self = this.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  _emscripten_bind_btQuadWord_setY_1(self, y);
};;

btQuadWord.prototype['setZ'] = btQuadWord.prototype.setZ = /** @suppress {undefinedVars, duplicate} */function(z) {
  var self = this.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_btQuadWord_setZ_1(self, z);
};;

btQuadWord.prototype['setW'] = btQuadWord.prototype.setW = /** @suppress {undefinedVars, duplicate} */function(w) {
  var self = this.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  _emscripten_bind_btQuadWord_setW_1(self, w);
};;

  btQuadWord.prototype['__destroy__'] = btQuadWord.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btQuadWord___destroy___0(self);
};
// btCylinderShape
/** @suppress {undefinedVars, duplicate} */function btCylinderShape(halfExtents) {
  if (halfExtents && typeof halfExtents === 'object') halfExtents = halfExtents.ptr;
  this.ptr = _emscripten_bind_btCylinderShape_btCylinderShape_1(halfExtents);
  getCache(btCylinderShape)[this.ptr] = this;
};;
btCylinderShape.prototype = Object.create(btCollisionShape.prototype);
btCylinderShape.prototype.constructor = btCylinderShape;
btCylinderShape.prototype.__class__ = btCylinderShape;
btCylinderShape.__cache__ = {};
Module['btCylinderShape'] = btCylinderShape;

btCylinderShape.prototype['setLocalScaling'] = btCylinderShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCylinderShape_setLocalScaling_1(self, scaling);
};;

btCylinderShape.prototype['calculateLocalInertia'] = btCylinderShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCylinderShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btCylinderShape.prototype['__destroy__'] = btCylinderShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCylinderShape___destroy___0(self);
};
// btActionInterface
/** @suppress {undefinedVars, duplicate} */function btActionInterface() { throw "cannot construct a btActionInterface, no constructor in IDL" }
btActionInterface.prototype = Object.create(WrapperObject.prototype);
btActionInterface.prototype.constructor = btActionInterface;
btActionInterface.prototype.__class__ = btActionInterface;
btActionInterface.__cache__ = {};
Module['btActionInterface'] = btActionInterface;

  btActionInterface.prototype['__destroy__'] = btActionInterface.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btActionInterface___destroy___0(self);
};
// btMatrix3x3
/** @suppress {undefinedVars, duplicate} */function btMatrix3x3() { throw "cannot construct a btMatrix3x3, no constructor in IDL" }
btMatrix3x3.prototype = Object.create(WrapperObject.prototype);
btMatrix3x3.prototype.constructor = btMatrix3x3;
btMatrix3x3.prototype.__class__ = btMatrix3x3;
btMatrix3x3.__cache__ = {};
Module['btMatrix3x3'] = btMatrix3x3;

btMatrix3x3.prototype['setEulerZYX'] = btMatrix3x3.prototype.setEulerZYX = /** @suppress {undefinedVars, duplicate} */function(ex, ey, ez) {
  var self = this.ptr;
  if (ex && typeof ex === 'object') ex = ex.ptr;
  if (ey && typeof ey === 'object') ey = ey.ptr;
  if (ez && typeof ez === 'object') ez = ez.ptr;
  _emscripten_bind_btMatrix3x3_setEulerZYX_3(self, ex, ey, ez);
};;

btMatrix3x3.prototype['getRotation'] = btMatrix3x3.prototype.getRotation = /** @suppress {undefinedVars, duplicate} */function(q) {
  var self = this.ptr;
  if (q && typeof q === 'object') q = q.ptr;
  _emscripten_bind_btMatrix3x3_getRotation_1(self, q);
};;

  btMatrix3x3.prototype['__destroy__'] = btMatrix3x3.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btMatrix3x3___destroy___0(self);
};
// ConcreteContactResultCallback
/** @suppress {undefinedVars, duplicate} */function ConcreteContactResultCallback() {
  this.ptr = _emscripten_bind_ConcreteContactResultCallback_ConcreteContactResultCallback_0();
  getCache(ConcreteContactResultCallback)[this.ptr] = this;
};;
ConcreteContactResultCallback.prototype = Object.create(ContactResultCallback.prototype);
ConcreteContactResultCallback.prototype.constructor = ConcreteContactResultCallback;
ConcreteContactResultCallback.prototype.__class__ = ConcreteContactResultCallback;
ConcreteContactResultCallback.__cache__ = {};
Module['ConcreteContactResultCallback'] = ConcreteContactResultCallback;

ConcreteContactResultCallback.prototype['addSingleResult'] = ConcreteContactResultCallback.prototype.addSingleResult = /** @suppress {undefinedVars, duplicate} */function(cp, colObj0Wrap, partId0, index0, colObj1Wrap, partId1, index1) {
  var self = this.ptr;
  if (cp && typeof cp === 'object') cp = cp.ptr;
  if (colObj0Wrap && typeof colObj0Wrap === 'object') colObj0Wrap = colObj0Wrap.ptr;
  if (partId0 && typeof partId0 === 'object') partId0 = partId0.ptr;
  if (index0 && typeof index0 === 'object') index0 = index0.ptr;
  if (colObj1Wrap && typeof colObj1Wrap === 'object') colObj1Wrap = colObj1Wrap.ptr;
  if (partId1 && typeof partId1 === 'object') partId1 = partId1.ptr;
  if (index1 && typeof index1 === 'object') index1 = index1.ptr;
  return _emscripten_bind_ConcreteContactResultCallback_addSingleResult_7(self, cp, colObj0Wrap, partId0, index0, colObj1Wrap, partId1, index1);
};;

  ConcreteContactResultCallback.prototype['__destroy__'] = ConcreteContactResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_ConcreteContactResultCallback___destroy___0(self);
};
// btBvhTriangleMeshShape
/** @suppress {undefinedVars, duplicate} */function btBvhTriangleMeshShape(meshInterface, useQuantizedAabbCompression, buildBvh) {
  if (meshInterface && typeof meshInterface === 'object') meshInterface = meshInterface.ptr;
  if (useQuantizedAabbCompression && typeof useQuantizedAabbCompression === 'object') useQuantizedAabbCompression = useQuantizedAabbCompression.ptr;
  if (buildBvh && typeof buildBvh === 'object') buildBvh = buildBvh.ptr;
  if (buildBvh === undefined) { this.ptr = _emscripten_bind_btBvhTriangleMeshShape_btBvhTriangleMeshShape_2(meshInterface, useQuantizedAabbCompression); getCache(btBvhTriangleMeshShape)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btBvhTriangleMeshShape_btBvhTriangleMeshShape_3(meshInterface, useQuantizedAabbCompression, buildBvh);
  getCache(btBvhTriangleMeshShape)[this.ptr] = this;
};;
btBvhTriangleMeshShape.prototype = Object.create(btTriangleMeshShape.prototype);
btBvhTriangleMeshShape.prototype.constructor = btBvhTriangleMeshShape;
btBvhTriangleMeshShape.prototype.__class__ = btBvhTriangleMeshShape;
btBvhTriangleMeshShape.__cache__ = {};
Module['btBvhTriangleMeshShape'] = btBvhTriangleMeshShape;

btBvhTriangleMeshShape.prototype['setLocalScaling'] = btBvhTriangleMeshShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btBvhTriangleMeshShape_setLocalScaling_1(self, scaling);
};;

btBvhTriangleMeshShape.prototype['calculateLocalInertia'] = btBvhTriangleMeshShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btBvhTriangleMeshShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btBvhTriangleMeshShape.prototype['__destroy__'] = btBvhTriangleMeshShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btBvhTriangleMeshShape___destroy___0(self);
};
// btDbvtBroadphase
/** @suppress {undefinedVars, duplicate} */function btDbvtBroadphase() {
  this.ptr = _emscripten_bind_btDbvtBroadphase_btDbvtBroadphase_0();
  getCache(btDbvtBroadphase)[this.ptr] = this;
};;
btDbvtBroadphase.prototype = Object.create(WrapperObject.prototype);
btDbvtBroadphase.prototype.constructor = btDbvtBroadphase;
btDbvtBroadphase.prototype.__class__ = btDbvtBroadphase;
btDbvtBroadphase.__cache__ = {};
Module['btDbvtBroadphase'] = btDbvtBroadphase;

  btDbvtBroadphase.prototype['__destroy__'] = btDbvtBroadphase.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDbvtBroadphase___destroy___0(self);
};
// VoidPtr
/** @suppress {undefinedVars, duplicate} */function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// btSequentialImpulseConstraintSolver
/** @suppress {undefinedVars, duplicate} */function btSequentialImpulseConstraintSolver() {
  this.ptr = _emscripten_bind_btSequentialImpulseConstraintSolver_btSequentialImpulseConstraintSolver_0();
  getCache(btSequentialImpulseConstraintSolver)[this.ptr] = this;
};;
btSequentialImpulseConstraintSolver.prototype = Object.create(WrapperObject.prototype);
btSequentialImpulseConstraintSolver.prototype.constructor = btSequentialImpulseConstraintSolver;
btSequentialImpulseConstraintSolver.prototype.__class__ = btSequentialImpulseConstraintSolver;
btSequentialImpulseConstraintSolver.__cache__ = {};
Module['btSequentialImpulseConstraintSolver'] = btSequentialImpulseConstraintSolver;

  btSequentialImpulseConstraintSolver.prototype['__destroy__'] = btSequentialImpulseConstraintSolver.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btSequentialImpulseConstraintSolver___destroy___0(self);
};
// btDispatcherInfo
/** @suppress {undefinedVars, duplicate} */function btDispatcherInfo() { throw "cannot construct a btDispatcherInfo, no constructor in IDL" }
btDispatcherInfo.prototype = Object.create(WrapperObject.prototype);
btDispatcherInfo.prototype.constructor = btDispatcherInfo;
btDispatcherInfo.prototype.__class__ = btDispatcherInfo;
btDispatcherInfo.__cache__ = {};
Module['btDispatcherInfo'] = btDispatcherInfo;

  btDispatcherInfo.prototype['get_m_timeStep'] = btDispatcherInfo.prototype.get_m_timeStep = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_timeStep_0(self);
};
    btDispatcherInfo.prototype['set_m_timeStep'] = btDispatcherInfo.prototype.set_m_timeStep = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_timeStep_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_timeStep', { get: btDispatcherInfo.prototype.get_m_timeStep, set: btDispatcherInfo.prototype.set_m_timeStep });
  btDispatcherInfo.prototype['get_m_stepCount'] = btDispatcherInfo.prototype.get_m_stepCount = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_stepCount_0(self);
};
    btDispatcherInfo.prototype['set_m_stepCount'] = btDispatcherInfo.prototype.set_m_stepCount = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_stepCount_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_stepCount', { get: btDispatcherInfo.prototype.get_m_stepCount, set: btDispatcherInfo.prototype.set_m_stepCount });
  btDispatcherInfo.prototype['get_m_dispatchFunc'] = btDispatcherInfo.prototype.get_m_dispatchFunc = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_dispatchFunc_0(self);
};
    btDispatcherInfo.prototype['set_m_dispatchFunc'] = btDispatcherInfo.prototype.set_m_dispatchFunc = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_dispatchFunc_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_dispatchFunc', { get: btDispatcherInfo.prototype.get_m_dispatchFunc, set: btDispatcherInfo.prototype.set_m_dispatchFunc });
  btDispatcherInfo.prototype['get_m_timeOfImpact'] = btDispatcherInfo.prototype.get_m_timeOfImpact = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_timeOfImpact_0(self);
};
    btDispatcherInfo.prototype['set_m_timeOfImpact'] = btDispatcherInfo.prototype.set_m_timeOfImpact = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_timeOfImpact_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_timeOfImpact', { get: btDispatcherInfo.prototype.get_m_timeOfImpact, set: btDispatcherInfo.prototype.set_m_timeOfImpact });
  btDispatcherInfo.prototype['get_m_useContinuous'] = btDispatcherInfo.prototype.get_m_useContinuous = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btDispatcherInfo_get_m_useContinuous_0(self));
};
    btDispatcherInfo.prototype['set_m_useContinuous'] = btDispatcherInfo.prototype.set_m_useContinuous = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_useContinuous_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_useContinuous', { get: btDispatcherInfo.prototype.get_m_useContinuous, set: btDispatcherInfo.prototype.set_m_useContinuous });
  btDispatcherInfo.prototype['get_m_enableSatConvex'] = btDispatcherInfo.prototype.get_m_enableSatConvex = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btDispatcherInfo_get_m_enableSatConvex_0(self));
};
    btDispatcherInfo.prototype['set_m_enableSatConvex'] = btDispatcherInfo.prototype.set_m_enableSatConvex = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_enableSatConvex_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_enableSatConvex', { get: btDispatcherInfo.prototype.get_m_enableSatConvex, set: btDispatcherInfo.prototype.set_m_enableSatConvex });
  btDispatcherInfo.prototype['get_m_enableSPU'] = btDispatcherInfo.prototype.get_m_enableSPU = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btDispatcherInfo_get_m_enableSPU_0(self));
};
    btDispatcherInfo.prototype['set_m_enableSPU'] = btDispatcherInfo.prototype.set_m_enableSPU = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_enableSPU_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_enableSPU', { get: btDispatcherInfo.prototype.get_m_enableSPU, set: btDispatcherInfo.prototype.set_m_enableSPU });
  btDispatcherInfo.prototype['get_m_useEpa'] = btDispatcherInfo.prototype.get_m_useEpa = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btDispatcherInfo_get_m_useEpa_0(self));
};
    btDispatcherInfo.prototype['set_m_useEpa'] = btDispatcherInfo.prototype.set_m_useEpa = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_useEpa_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_useEpa', { get: btDispatcherInfo.prototype.get_m_useEpa, set: btDispatcherInfo.prototype.set_m_useEpa });
  btDispatcherInfo.prototype['get_m_allowedCcdPenetration'] = btDispatcherInfo.prototype.get_m_allowedCcdPenetration = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_allowedCcdPenetration_0(self);
};
    btDispatcherInfo.prototype['set_m_allowedCcdPenetration'] = btDispatcherInfo.prototype.set_m_allowedCcdPenetration = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_allowedCcdPenetration_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_allowedCcdPenetration', { get: btDispatcherInfo.prototype.get_m_allowedCcdPenetration, set: btDispatcherInfo.prototype.set_m_allowedCcdPenetration });
  btDispatcherInfo.prototype['get_m_useConvexConservativeDistanceUtil'] = btDispatcherInfo.prototype.get_m_useConvexConservativeDistanceUtil = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btDispatcherInfo_get_m_useConvexConservativeDistanceUtil_0(self));
};
    btDispatcherInfo.prototype['set_m_useConvexConservativeDistanceUtil'] = btDispatcherInfo.prototype.set_m_useConvexConservativeDistanceUtil = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_useConvexConservativeDistanceUtil_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_useConvexConservativeDistanceUtil', { get: btDispatcherInfo.prototype.get_m_useConvexConservativeDistanceUtil, set: btDispatcherInfo.prototype.set_m_useConvexConservativeDistanceUtil });
  btDispatcherInfo.prototype['get_m_convexConservativeDistanceThreshold'] = btDispatcherInfo.prototype.get_m_convexConservativeDistanceThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btDispatcherInfo_get_m_convexConservativeDistanceThreshold_0(self);
};
    btDispatcherInfo.prototype['set_m_convexConservativeDistanceThreshold'] = btDispatcherInfo.prototype.set_m_convexConservativeDistanceThreshold = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDispatcherInfo_set_m_convexConservativeDistanceThreshold_1(self, arg0);
};
    Object.defineProperty(btDispatcherInfo.prototype, 'm_convexConservativeDistanceThreshold', { get: btDispatcherInfo.prototype.get_m_convexConservativeDistanceThreshold, set: btDispatcherInfo.prototype.set_m_convexConservativeDistanceThreshold });
  btDispatcherInfo.prototype['__destroy__'] = btDispatcherInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDispatcherInfo___destroy___0(self);
};
// btCollisionConfiguration
/** @suppress {undefinedVars, duplicate} */function btCollisionConfiguration() { throw "cannot construct a btCollisionConfiguration, no constructor in IDL" }
btCollisionConfiguration.prototype = Object.create(WrapperObject.prototype);
btCollisionConfiguration.prototype.constructor = btCollisionConfiguration;
btCollisionConfiguration.prototype.__class__ = btCollisionConfiguration;
btCollisionConfiguration.__cache__ = {};
Module['btCollisionConfiguration'] = btCollisionConfiguration;

  btCollisionConfiguration.prototype['__destroy__'] = btCollisionConfiguration.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCollisionConfiguration___destroy___0(self);
};
// btAxisSweep3
/** @suppress {undefinedVars, duplicate} */function btAxisSweep3(worldAabbMin, worldAabbMax, maxHandles, pairCache, disableRaycastAccelerator) {
  if (worldAabbMin && typeof worldAabbMin === 'object') worldAabbMin = worldAabbMin.ptr;
  if (worldAabbMax && typeof worldAabbMax === 'object') worldAabbMax = worldAabbMax.ptr;
  if (maxHandles && typeof maxHandles === 'object') maxHandles = maxHandles.ptr;
  if (pairCache && typeof pairCache === 'object') pairCache = pairCache.ptr;
  if (disableRaycastAccelerator && typeof disableRaycastAccelerator === 'object') disableRaycastAccelerator = disableRaycastAccelerator.ptr;
  if (maxHandles === undefined) { this.ptr = _emscripten_bind_btAxisSweep3_btAxisSweep3_2(worldAabbMin, worldAabbMax); getCache(btAxisSweep3)[this.ptr] = this;return }
  if (pairCache === undefined) { this.ptr = _emscripten_bind_btAxisSweep3_btAxisSweep3_3(worldAabbMin, worldAabbMax, maxHandles); getCache(btAxisSweep3)[this.ptr] = this;return }
  if (disableRaycastAccelerator === undefined) { this.ptr = _emscripten_bind_btAxisSweep3_btAxisSweep3_4(worldAabbMin, worldAabbMax, maxHandles, pairCache); getCache(btAxisSweep3)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btAxisSweep3_btAxisSweep3_5(worldAabbMin, worldAabbMax, maxHandles, pairCache, disableRaycastAccelerator);
  getCache(btAxisSweep3)[this.ptr] = this;
};;
btAxisSweep3.prototype = Object.create(WrapperObject.prototype);
btAxisSweep3.prototype.constructor = btAxisSweep3;
btAxisSweep3.prototype.__class__ = btAxisSweep3;
btAxisSweep3.__cache__ = {};
Module['btAxisSweep3'] = btAxisSweep3;

  btAxisSweep3.prototype['__destroy__'] = btAxisSweep3.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btAxisSweep3___destroy___0(self);
};
// btPairCachingGhostObject
/** @suppress {undefinedVars, duplicate} */function btPairCachingGhostObject() {
  this.ptr = _emscripten_bind_btPairCachingGhostObject_btPairCachingGhostObject_0();
  getCache(btPairCachingGhostObject)[this.ptr] = this;
};;
btPairCachingGhostObject.prototype = Object.create(btGhostObject.prototype);
btPairCachingGhostObject.prototype.constructor = btPairCachingGhostObject;
btPairCachingGhostObject.prototype.__class__ = btPairCachingGhostObject;
btPairCachingGhostObject.__cache__ = {};
Module['btPairCachingGhostObject'] = btPairCachingGhostObject;

btPairCachingGhostObject.prototype['setAnisotropicFriction'] = btPairCachingGhostObject.prototype.setAnisotropicFriction = /** @suppress {undefinedVars, duplicate} */function(anisotropicFriction, frictionMode) {
  var self = this.ptr;
  if (anisotropicFriction && typeof anisotropicFriction === 'object') anisotropicFriction = anisotropicFriction.ptr;
  if (frictionMode && typeof frictionMode === 'object') frictionMode = frictionMode.ptr;
  _emscripten_bind_btPairCachingGhostObject_setAnisotropicFriction_2(self, anisotropicFriction, frictionMode);
};;

btPairCachingGhostObject.prototype['getCollisionShape'] = btPairCachingGhostObject.prototype.getCollisionShape = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPairCachingGhostObject_getCollisionShape_0(self), btCollisionShape);
};;

btPairCachingGhostObject.prototype['setContactProcessingThreshold'] = btPairCachingGhostObject.prototype.setContactProcessingThreshold = /** @suppress {undefinedVars, duplicate} */function(contactProcessingThreshold) {
  var self = this.ptr;
  if (contactProcessingThreshold && typeof contactProcessingThreshold === 'object') contactProcessingThreshold = contactProcessingThreshold.ptr;
  _emscripten_bind_btPairCachingGhostObject_setContactProcessingThreshold_1(self, contactProcessingThreshold);
};;

btPairCachingGhostObject.prototype['setActivationState'] = btPairCachingGhostObject.prototype.setActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btPairCachingGhostObject_setActivationState_1(self, newState);
};;

btPairCachingGhostObject.prototype['forceActivationState'] = btPairCachingGhostObject.prototype.forceActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btPairCachingGhostObject_forceActivationState_1(self, newState);
};;

btPairCachingGhostObject.prototype['activate'] = btPairCachingGhostObject.prototype.activate = /** @suppress {undefinedVars, duplicate} */function(forceActivation) {
  var self = this.ptr;
  if (forceActivation && typeof forceActivation === 'object') forceActivation = forceActivation.ptr;
  if (forceActivation === undefined) { _emscripten_bind_btPairCachingGhostObject_activate_0(self);  return }
  _emscripten_bind_btPairCachingGhostObject_activate_1(self, forceActivation);
};;

btPairCachingGhostObject.prototype['isActive'] = btPairCachingGhostObject.prototype.isActive = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btPairCachingGhostObject_isActive_0(self));
};;

btPairCachingGhostObject.prototype['isKinematicObject'] = btPairCachingGhostObject.prototype.isKinematicObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btPairCachingGhostObject_isKinematicObject_0(self));
};;

btPairCachingGhostObject.prototype['setRestitution'] = btPairCachingGhostObject.prototype.setRestitution = /** @suppress {undefinedVars, duplicate} */function(rest) {
  var self = this.ptr;
  if (rest && typeof rest === 'object') rest = rest.ptr;
  _emscripten_bind_btPairCachingGhostObject_setRestitution_1(self, rest);
};;

btPairCachingGhostObject.prototype['setFriction'] = btPairCachingGhostObject.prototype.setFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btPairCachingGhostObject_setFriction_1(self, frict);
};;

btPairCachingGhostObject.prototype['setRollingFriction'] = btPairCachingGhostObject.prototype.setRollingFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btPairCachingGhostObject_setRollingFriction_1(self, frict);
};;

btPairCachingGhostObject.prototype['getWorldTransform'] = btPairCachingGhostObject.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPairCachingGhostObject_getWorldTransform_0(self), btTransform);
};;

btPairCachingGhostObject.prototype['getCollisionFlags'] = btPairCachingGhostObject.prototype.getCollisionFlags = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btPairCachingGhostObject_getCollisionFlags_0(self);
};;

btPairCachingGhostObject.prototype['setCollisionFlags'] = btPairCachingGhostObject.prototype.setCollisionFlags = /** @suppress {undefinedVars, duplicate} */function(flags) {
  var self = this.ptr;
  if (flags && typeof flags === 'object') flags = flags.ptr;
  _emscripten_bind_btPairCachingGhostObject_setCollisionFlags_1(self, flags);
};;

btPairCachingGhostObject.prototype['setWorldTransform'] = btPairCachingGhostObject.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btPairCachingGhostObject_setWorldTransform_1(self, worldTrans);
};;

btPairCachingGhostObject.prototype['setCollisionShape'] = btPairCachingGhostObject.prototype.setCollisionShape = /** @suppress {undefinedVars, duplicate} */function(collisionShape) {
  var self = this.ptr;
  if (collisionShape && typeof collisionShape === 'object') collisionShape = collisionShape.ptr;
  _emscripten_bind_btPairCachingGhostObject_setCollisionShape_1(self, collisionShape);
};;

btPairCachingGhostObject.prototype['setCcdMotionThreshold'] = btPairCachingGhostObject.prototype.setCcdMotionThreshold = /** @suppress {undefinedVars, duplicate} */function(ccdMotionThreshold) {
  var self = this.ptr;
  if (ccdMotionThreshold && typeof ccdMotionThreshold === 'object') ccdMotionThreshold = ccdMotionThreshold.ptr;
  _emscripten_bind_btPairCachingGhostObject_setCcdMotionThreshold_1(self, ccdMotionThreshold);
};;

btPairCachingGhostObject.prototype['setCcdSweptSphereRadius'] = btPairCachingGhostObject.prototype.setCcdSweptSphereRadius = /** @suppress {undefinedVars, duplicate} */function(radius) {
  var self = this.ptr;
  if (radius && typeof radius === 'object') radius = radius.ptr;
  _emscripten_bind_btPairCachingGhostObject_setCcdSweptSphereRadius_1(self, radius);
};;

btPairCachingGhostObject.prototype['getNumOverlappingObjects'] = btPairCachingGhostObject.prototype.getNumOverlappingObjects = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btPairCachingGhostObject_getNumOverlappingObjects_0(self);
};;

btPairCachingGhostObject.prototype['getOverlappingObject'] = btPairCachingGhostObject.prototype.getOverlappingObject = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btPairCachingGhostObject_getOverlappingObject_1(self, index), btCollisionObject);
};;

  btPairCachingGhostObject.prototype['__destroy__'] = btPairCachingGhostObject.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btPairCachingGhostObject___destroy___0(self);
};
// btRigidBodyConstructionInfo
/** @suppress {undefinedVars, duplicate} */function btRigidBodyConstructionInfo(mass, motionState, collisionShape, localInertia) {
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (motionState && typeof motionState === 'object') motionState = motionState.ptr;
  if (collisionShape && typeof collisionShape === 'object') collisionShape = collisionShape.ptr;
  if (localInertia && typeof localInertia === 'object') localInertia = localInertia.ptr;
  if (localInertia === undefined) { this.ptr = _emscripten_bind_btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_3(mass, motionState, collisionShape); getCache(btRigidBodyConstructionInfo)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_4(mass, motionState, collisionShape, localInertia);
  getCache(btRigidBodyConstructionInfo)[this.ptr] = this;
};;
btRigidBodyConstructionInfo.prototype = Object.create(WrapperObject.prototype);
btRigidBodyConstructionInfo.prototype.constructor = btRigidBodyConstructionInfo;
btRigidBodyConstructionInfo.prototype.__class__ = btRigidBodyConstructionInfo;
btRigidBodyConstructionInfo.__cache__ = {};
Module['btRigidBodyConstructionInfo'] = btRigidBodyConstructionInfo;

  btRigidBodyConstructionInfo.prototype['get_m_linearDamping'] = btRigidBodyConstructionInfo.prototype.get_m_linearDamping = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_linearDamping_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_linearDamping'] = btRigidBodyConstructionInfo.prototype.set_m_linearDamping = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_linearDamping_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_linearDamping', { get: btRigidBodyConstructionInfo.prototype.get_m_linearDamping, set: btRigidBodyConstructionInfo.prototype.set_m_linearDamping });
  btRigidBodyConstructionInfo.prototype['get_m_angularDamping'] = btRigidBodyConstructionInfo.prototype.get_m_angularDamping = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_angularDamping_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_angularDamping'] = btRigidBodyConstructionInfo.prototype.set_m_angularDamping = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_angularDamping_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_angularDamping', { get: btRigidBodyConstructionInfo.prototype.get_m_angularDamping, set: btRigidBodyConstructionInfo.prototype.set_m_angularDamping });
  btRigidBodyConstructionInfo.prototype['get_m_friction'] = btRigidBodyConstructionInfo.prototype.get_m_friction = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_friction_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_friction'] = btRigidBodyConstructionInfo.prototype.set_m_friction = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_friction_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_friction', { get: btRigidBodyConstructionInfo.prototype.get_m_friction, set: btRigidBodyConstructionInfo.prototype.set_m_friction });
  btRigidBodyConstructionInfo.prototype['get_m_rollingFriction'] = btRigidBodyConstructionInfo.prototype.get_m_rollingFriction = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_rollingFriction_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_rollingFriction'] = btRigidBodyConstructionInfo.prototype.set_m_rollingFriction = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_rollingFriction_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_rollingFriction', { get: btRigidBodyConstructionInfo.prototype.get_m_rollingFriction, set: btRigidBodyConstructionInfo.prototype.set_m_rollingFriction });
  btRigidBodyConstructionInfo.prototype['get_m_restitution'] = btRigidBodyConstructionInfo.prototype.get_m_restitution = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_restitution_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_restitution'] = btRigidBodyConstructionInfo.prototype.set_m_restitution = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_restitution_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_restitution', { get: btRigidBodyConstructionInfo.prototype.get_m_restitution, set: btRigidBodyConstructionInfo.prototype.set_m_restitution });
  btRigidBodyConstructionInfo.prototype['get_m_linearSleepingThreshold'] = btRigidBodyConstructionInfo.prototype.get_m_linearSleepingThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_linearSleepingThreshold_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_linearSleepingThreshold'] = btRigidBodyConstructionInfo.prototype.set_m_linearSleepingThreshold = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_linearSleepingThreshold_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_linearSleepingThreshold', { get: btRigidBodyConstructionInfo.prototype.get_m_linearSleepingThreshold, set: btRigidBodyConstructionInfo.prototype.set_m_linearSleepingThreshold });
  btRigidBodyConstructionInfo.prototype['get_m_angularSleepingThreshold'] = btRigidBodyConstructionInfo.prototype.get_m_angularSleepingThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_angularSleepingThreshold_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_angularSleepingThreshold'] = btRigidBodyConstructionInfo.prototype.set_m_angularSleepingThreshold = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_angularSleepingThreshold_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_angularSleepingThreshold', { get: btRigidBodyConstructionInfo.prototype.get_m_angularSleepingThreshold, set: btRigidBodyConstructionInfo.prototype.set_m_angularSleepingThreshold });
  btRigidBodyConstructionInfo.prototype['get_m_additionalDamping'] = btRigidBodyConstructionInfo.prototype.get_m_additionalDamping = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btRigidBodyConstructionInfo_get_m_additionalDamping_0(self));
};
    btRigidBodyConstructionInfo.prototype['set_m_additionalDamping'] = btRigidBodyConstructionInfo.prototype.set_m_additionalDamping = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_additionalDamping_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_additionalDamping', { get: btRigidBodyConstructionInfo.prototype.get_m_additionalDamping, set: btRigidBodyConstructionInfo.prototype.set_m_additionalDamping });
  btRigidBodyConstructionInfo.prototype['get_m_additionalDampingFactor'] = btRigidBodyConstructionInfo.prototype.get_m_additionalDampingFactor = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_additionalDampingFactor_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_additionalDampingFactor'] = btRigidBodyConstructionInfo.prototype.set_m_additionalDampingFactor = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_additionalDampingFactor_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_additionalDampingFactor', { get: btRigidBodyConstructionInfo.prototype.get_m_additionalDampingFactor, set: btRigidBodyConstructionInfo.prototype.set_m_additionalDampingFactor });
  btRigidBodyConstructionInfo.prototype['get_m_additionalLinearDampingThresholdSqr'] = btRigidBodyConstructionInfo.prototype.get_m_additionalLinearDampingThresholdSqr = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_additionalLinearDampingThresholdSqr_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_additionalLinearDampingThresholdSqr'] = btRigidBodyConstructionInfo.prototype.set_m_additionalLinearDampingThresholdSqr = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_additionalLinearDampingThresholdSqr_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_additionalLinearDampingThresholdSqr', { get: btRigidBodyConstructionInfo.prototype.get_m_additionalLinearDampingThresholdSqr, set: btRigidBodyConstructionInfo.prototype.set_m_additionalLinearDampingThresholdSqr });
  btRigidBodyConstructionInfo.prototype['get_m_additionalAngularDampingThresholdSqr'] = btRigidBodyConstructionInfo.prototype.get_m_additionalAngularDampingThresholdSqr = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_additionalAngularDampingThresholdSqr_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_additionalAngularDampingThresholdSqr'] = btRigidBodyConstructionInfo.prototype.set_m_additionalAngularDampingThresholdSqr = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_additionalAngularDampingThresholdSqr_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_additionalAngularDampingThresholdSqr', { get: btRigidBodyConstructionInfo.prototype.get_m_additionalAngularDampingThresholdSqr, set: btRigidBodyConstructionInfo.prototype.set_m_additionalAngularDampingThresholdSqr });
  btRigidBodyConstructionInfo.prototype['get_m_additionalAngularDampingFactor'] = btRigidBodyConstructionInfo.prototype.get_m_additionalAngularDampingFactor = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBodyConstructionInfo_get_m_additionalAngularDampingFactor_0(self);
};
    btRigidBodyConstructionInfo.prototype['set_m_additionalAngularDampingFactor'] = btRigidBodyConstructionInfo.prototype.set_m_additionalAngularDampingFactor = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo_set_m_additionalAngularDampingFactor_1(self, arg0);
};
    Object.defineProperty(btRigidBodyConstructionInfo.prototype, 'm_additionalAngularDampingFactor', { get: btRigidBodyConstructionInfo.prototype.get_m_additionalAngularDampingFactor, set: btRigidBodyConstructionInfo.prototype.set_m_additionalAngularDampingFactor });
  btRigidBodyConstructionInfo.prototype['__destroy__'] = btRigidBodyConstructionInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btRigidBodyConstructionInfo___destroy___0(self);
};
// btManifoldPoint
/** @suppress {undefinedVars, duplicate} */function btManifoldPoint() { throw "cannot construct a btManifoldPoint, no constructor in IDL" }
btManifoldPoint.prototype = Object.create(WrapperObject.prototype);
btManifoldPoint.prototype.constructor = btManifoldPoint;
btManifoldPoint.prototype.__class__ = btManifoldPoint;
btManifoldPoint.__cache__ = {};
Module['btManifoldPoint'] = btManifoldPoint;

btManifoldPoint.prototype['getPositionWorldOnA'] = btManifoldPoint.prototype.getPositionWorldOnA = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_getPositionWorldOnA_0(self), btVector3);
};;

btManifoldPoint.prototype['getPositionWorldOnB'] = btManifoldPoint.prototype.getPositionWorldOnB = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_getPositionWorldOnB_0(self), btVector3);
};;

btManifoldPoint.prototype['getDistance'] = btManifoldPoint.prototype.getDistance = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btManifoldPoint_getDistance_0(self);
};;

  btManifoldPoint.prototype['get_m_localPointA'] = btManifoldPoint.prototype.get_m_localPointA = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_get_m_localPointA_0(self), btVector3);
};
    btManifoldPoint.prototype['set_m_localPointA'] = btManifoldPoint.prototype.set_m_localPointA = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btManifoldPoint_set_m_localPointA_1(self, arg0);
};
    Object.defineProperty(btManifoldPoint.prototype, 'm_localPointA', { get: btManifoldPoint.prototype.get_m_localPointA, set: btManifoldPoint.prototype.set_m_localPointA });
  btManifoldPoint.prototype['get_m_localPointB'] = btManifoldPoint.prototype.get_m_localPointB = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_get_m_localPointB_0(self), btVector3);
};
    btManifoldPoint.prototype['set_m_localPointB'] = btManifoldPoint.prototype.set_m_localPointB = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btManifoldPoint_set_m_localPointB_1(self, arg0);
};
    Object.defineProperty(btManifoldPoint.prototype, 'm_localPointB', { get: btManifoldPoint.prototype.get_m_localPointB, set: btManifoldPoint.prototype.set_m_localPointB });
  btManifoldPoint.prototype['get_m_positionWorldOnB'] = btManifoldPoint.prototype.get_m_positionWorldOnB = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_get_m_positionWorldOnB_0(self), btVector3);
};
    btManifoldPoint.prototype['set_m_positionWorldOnB'] = btManifoldPoint.prototype.set_m_positionWorldOnB = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btManifoldPoint_set_m_positionWorldOnB_1(self, arg0);
};
    Object.defineProperty(btManifoldPoint.prototype, 'm_positionWorldOnB', { get: btManifoldPoint.prototype.get_m_positionWorldOnB, set: btManifoldPoint.prototype.set_m_positionWorldOnB });
  btManifoldPoint.prototype['get_m_positionWorldOnA'] = btManifoldPoint.prototype.get_m_positionWorldOnA = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_get_m_positionWorldOnA_0(self), btVector3);
};
    btManifoldPoint.prototype['set_m_positionWorldOnA'] = btManifoldPoint.prototype.set_m_positionWorldOnA = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btManifoldPoint_set_m_positionWorldOnA_1(self, arg0);
};
    Object.defineProperty(btManifoldPoint.prototype, 'm_positionWorldOnA', { get: btManifoldPoint.prototype.get_m_positionWorldOnA, set: btManifoldPoint.prototype.set_m_positionWorldOnA });
  btManifoldPoint.prototype['get_m_normalWorldOnB'] = btManifoldPoint.prototype.get_m_normalWorldOnB = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btManifoldPoint_get_m_normalWorldOnB_0(self), btVector3);
};
    btManifoldPoint.prototype['set_m_normalWorldOnB'] = btManifoldPoint.prototype.set_m_normalWorldOnB = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btManifoldPoint_set_m_normalWorldOnB_1(self, arg0);
};
    Object.defineProperty(btManifoldPoint.prototype, 'm_normalWorldOnB', { get: btManifoldPoint.prototype.get_m_normalWorldOnB, set: btManifoldPoint.prototype.set_m_normalWorldOnB });
  btManifoldPoint.prototype['__destroy__'] = btManifoldPoint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btManifoldPoint___destroy___0(self);
};
// btBroadphaseInterface
/** @suppress {undefinedVars, duplicate} */function btBroadphaseInterface() { throw "cannot construct a btBroadphaseInterface, no constructor in IDL" }
btBroadphaseInterface.prototype = Object.create(WrapperObject.prototype);
btBroadphaseInterface.prototype.constructor = btBroadphaseInterface;
btBroadphaseInterface.prototype.__class__ = btBroadphaseInterface;
btBroadphaseInterface.__cache__ = {};
Module['btBroadphaseInterface'] = btBroadphaseInterface;

  btBroadphaseInterface.prototype['__destroy__'] = btBroadphaseInterface.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btBroadphaseInterface___destroy___0(self);
};
// btDiscreteDynamicsWorld
/** @suppress {undefinedVars, duplicate} */function btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration) {
  if (dispatcher && typeof dispatcher === 'object') dispatcher = dispatcher.ptr;
  if (pairCache && typeof pairCache === 'object') pairCache = pairCache.ptr;
  if (constraintSolver && typeof constraintSolver === 'object') constraintSolver = constraintSolver.ptr;
  if (collisionConfiguration && typeof collisionConfiguration === 'object') collisionConfiguration = collisionConfiguration.ptr;
  this.ptr = _emscripten_bind_btDiscreteDynamicsWorld_btDiscreteDynamicsWorld_4(dispatcher, pairCache, constraintSolver, collisionConfiguration);
  getCache(btDiscreteDynamicsWorld)[this.ptr] = this;
};;
btDiscreteDynamicsWorld.prototype = Object.create(btDynamicsWorld.prototype);
btDiscreteDynamicsWorld.prototype.constructor = btDiscreteDynamicsWorld;
btDiscreteDynamicsWorld.prototype.__class__ = btDiscreteDynamicsWorld;
btDiscreteDynamicsWorld.__cache__ = {};
Module['btDiscreteDynamicsWorld'] = btDiscreteDynamicsWorld;

btDiscreteDynamicsWorld.prototype['setGravity'] = btDiscreteDynamicsWorld.prototype.setGravity = /** @suppress {undefinedVars, duplicate} */function(gravity) {
  var self = this.ptr;
  if (gravity && typeof gravity === 'object') gravity = gravity.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_setGravity_1(self, gravity);
};;

btDiscreteDynamicsWorld.prototype['getGravity'] = btDiscreteDynamicsWorld.prototype.getGravity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getGravity_0(self), btVector3);
};;

btDiscreteDynamicsWorld.prototype['addRigidBody'] = btDiscreteDynamicsWorld.prototype.addRigidBody = /** @suppress {undefinedVars, duplicate} */function(body, group, mask) {
  var self = this.ptr;
  if (body && typeof body === 'object') body = body.ptr;
  if (group && typeof group === 'object') group = group.ptr;
  if (mask && typeof mask === 'object') mask = mask.ptr;
  if (group === undefined) { _emscripten_bind_btDiscreteDynamicsWorld_addRigidBody_1(self, body);  return }
  if (mask === undefined) { _emscripten_bind_btDiscreteDynamicsWorld_addRigidBody_2(self, body, group);  return }
  _emscripten_bind_btDiscreteDynamicsWorld_addRigidBody_3(self, body, group, mask);
};;

btDiscreteDynamicsWorld.prototype['removeRigidBody'] = btDiscreteDynamicsWorld.prototype.removeRigidBody = /** @suppress {undefinedVars, duplicate} */function(body) {
  var self = this.ptr;
  if (body && typeof body === 'object') body = body.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_removeRigidBody_1(self, body);
};;

btDiscreteDynamicsWorld.prototype['addConstraint'] = btDiscreteDynamicsWorld.prototype.addConstraint = /** @suppress {undefinedVars, duplicate} */function(constraint, disableCollisionsBetweenLinkedBodies) {
  var self = this.ptr;
  if (constraint && typeof constraint === 'object') constraint = constraint.ptr;
  if (disableCollisionsBetweenLinkedBodies && typeof disableCollisionsBetweenLinkedBodies === 'object') disableCollisionsBetweenLinkedBodies = disableCollisionsBetweenLinkedBodies.ptr;
  if (disableCollisionsBetweenLinkedBodies === undefined) { _emscripten_bind_btDiscreteDynamicsWorld_addConstraint_1(self, constraint);  return }
  _emscripten_bind_btDiscreteDynamicsWorld_addConstraint_2(self, constraint, disableCollisionsBetweenLinkedBodies);
};;

btDiscreteDynamicsWorld.prototype['removeConstraint'] = btDiscreteDynamicsWorld.prototype.removeConstraint = /** @suppress {undefinedVars, duplicate} */function(constraint) {
  var self = this.ptr;
  if (constraint && typeof constraint === 'object') constraint = constraint.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_removeConstraint_1(self, constraint);
};;

btDiscreteDynamicsWorld.prototype['stepSimulation'] = btDiscreteDynamicsWorld.prototype.stepSimulation = /** @suppress {undefinedVars, duplicate} */function(timeStep, maxSubSteps, fixedTimeStep) {
  var self = this.ptr;
  if (timeStep && typeof timeStep === 'object') timeStep = timeStep.ptr;
  if (maxSubSteps && typeof maxSubSteps === 'object') maxSubSteps = maxSubSteps.ptr;
  if (fixedTimeStep && typeof fixedTimeStep === 'object') fixedTimeStep = fixedTimeStep.ptr;
  if (maxSubSteps === undefined) { return _emscripten_bind_btDiscreteDynamicsWorld_stepSimulation_1(self, timeStep) }
  if (fixedTimeStep === undefined) { return _emscripten_bind_btDiscreteDynamicsWorld_stepSimulation_2(self, timeStep, maxSubSteps) }
  return _emscripten_bind_btDiscreteDynamicsWorld_stepSimulation_3(self, timeStep, maxSubSteps, fixedTimeStep);
};;

btDiscreteDynamicsWorld.prototype['getDispatcher'] = btDiscreteDynamicsWorld.prototype.getDispatcher = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getDispatcher_0(self), btDispatcher);
};;

btDiscreteDynamicsWorld.prototype['rayTest'] = btDiscreteDynamicsWorld.prototype.rayTest = /** @suppress {undefinedVars, duplicate} */function(rayFromWorld, rayToWorld, resultCallback) {
  var self = this.ptr;
  if (rayFromWorld && typeof rayFromWorld === 'object') rayFromWorld = rayFromWorld.ptr;
  if (rayToWorld && typeof rayToWorld === 'object') rayToWorld = rayToWorld.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_rayTest_3(self, rayFromWorld, rayToWorld, resultCallback);
};;

btDiscreteDynamicsWorld.prototype['getPairCache'] = btDiscreteDynamicsWorld.prototype.getPairCache = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getPairCache_0(self), btOverlappingPairCache);
};;

btDiscreteDynamicsWorld.prototype['getDispatchInfo'] = btDiscreteDynamicsWorld.prototype.getDispatchInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getDispatchInfo_0(self), btDispatcherInfo);
};;

btDiscreteDynamicsWorld.prototype['addCollisionObject'] = btDiscreteDynamicsWorld.prototype.addCollisionObject = /** @suppress {undefinedVars, duplicate} */function(collisionObject, collisionFilterGroup, collisionFilterMask) {
  var self = this.ptr;
  if (collisionObject && typeof collisionObject === 'object') collisionObject = collisionObject.ptr;
  if (collisionFilterGroup && typeof collisionFilterGroup === 'object') collisionFilterGroup = collisionFilterGroup.ptr;
  if (collisionFilterMask && typeof collisionFilterMask === 'object') collisionFilterMask = collisionFilterMask.ptr;
  if (collisionFilterGroup === undefined) { _emscripten_bind_btDiscreteDynamicsWorld_addCollisionObject_1(self, collisionObject);  return }
  if (collisionFilterMask === undefined) { _emscripten_bind_btDiscreteDynamicsWorld_addCollisionObject_2(self, collisionObject, collisionFilterGroup);  return }
  _emscripten_bind_btDiscreteDynamicsWorld_addCollisionObject_3(self, collisionObject, collisionFilterGroup, collisionFilterMask);
};;

btDiscreteDynamicsWorld.prototype['getBroadphase'] = btDiscreteDynamicsWorld.prototype.getBroadphase = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getBroadphase_0(self), btBroadphaseInterface);
};;

btDiscreteDynamicsWorld.prototype['convexSweepTest'] = btDiscreteDynamicsWorld.prototype.convexSweepTest = /** @suppress {undefinedVars, duplicate} */function(castShape, from, to, resultCallback, allowedCcdPenetration) {
  var self = this.ptr;
  if (castShape && typeof castShape === 'object') castShape = castShape.ptr;
  if (from && typeof from === 'object') from = from.ptr;
  if (to && typeof to === 'object') to = to.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  if (allowedCcdPenetration && typeof allowedCcdPenetration === 'object') allowedCcdPenetration = allowedCcdPenetration.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_convexSweepTest_5(self, castShape, from, to, resultCallback, allowedCcdPenetration);
};;

btDiscreteDynamicsWorld.prototype['contactPairTest'] = btDiscreteDynamicsWorld.prototype.contactPairTest = /** @suppress {undefinedVars, duplicate} */function(colObjA, colObjB, resultCallback) {
  var self = this.ptr;
  if (colObjA && typeof colObjA === 'object') colObjA = colObjA.ptr;
  if (colObjB && typeof colObjB === 'object') colObjB = colObjB.ptr;
  if (resultCallback && typeof resultCallback === 'object') resultCallback = resultCallback.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_contactPairTest_3(self, colObjA, colObjB, resultCallback);
};;

btDiscreteDynamicsWorld.prototype['addAction'] = btDiscreteDynamicsWorld.prototype.addAction = /** @suppress {undefinedVars, duplicate} */function(action) {
  var self = this.ptr;
  if (action && typeof action === 'object') action = action.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld_addAction_1(self, action);
};;

btDiscreteDynamicsWorld.prototype['getSolverInfo'] = btDiscreteDynamicsWorld.prototype.getSolverInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDiscreteDynamicsWorld_getSolverInfo_0(self), btContactSolverInfo);
};;

  btDiscreteDynamicsWorld.prototype['__destroy__'] = btDiscreteDynamicsWorld.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDiscreteDynamicsWorld___destroy___0(self);
};
// btConvexTriangleMeshShape
/** @suppress {undefinedVars, duplicate} */function btConvexTriangleMeshShape(meshInterface, calcAabb) {
  if (meshInterface && typeof meshInterface === 'object') meshInterface = meshInterface.ptr;
  if (calcAabb && typeof calcAabb === 'object') calcAabb = calcAabb.ptr;
  if (calcAabb === undefined) { this.ptr = _emscripten_bind_btConvexTriangleMeshShape_btConvexTriangleMeshShape_1(meshInterface); getCache(btConvexTriangleMeshShape)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btConvexTriangleMeshShape_btConvexTriangleMeshShape_2(meshInterface, calcAabb);
  getCache(btConvexTriangleMeshShape)[this.ptr] = this;
};;
btConvexTriangleMeshShape.prototype = Object.create(btConvexShape.prototype);
btConvexTriangleMeshShape.prototype.constructor = btConvexTriangleMeshShape;
btConvexTriangleMeshShape.prototype.__class__ = btConvexTriangleMeshShape;
btConvexTriangleMeshShape.__cache__ = {};
Module['btConvexTriangleMeshShape'] = btConvexTriangleMeshShape;

btConvexTriangleMeshShape.prototype['setLocalScaling'] = btConvexTriangleMeshShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConvexTriangleMeshShape_setLocalScaling_1(self, scaling);
};;

btConvexTriangleMeshShape.prototype['calculateLocalInertia'] = btConvexTriangleMeshShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConvexTriangleMeshShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btConvexTriangleMeshShape.prototype['__destroy__'] = btConvexTriangleMeshShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConvexTriangleMeshShape___destroy___0(self);
};
// btPoint2PointConstraint
/** @suppress {undefinedVars, duplicate} */function btPoint2PointConstraint(rbA, rbB, pivotInA, pivotInB) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (pivotInA && typeof pivotInA === 'object') pivotInA = pivotInA.ptr;
  if (pivotInB && typeof pivotInB === 'object') pivotInB = pivotInB.ptr;
  if (pivotInA === undefined) { this.ptr = _emscripten_bind_btPoint2PointConstraint_btPoint2PointConstraint_2(rbA, rbB); getCache(btPoint2PointConstraint)[this.ptr] = this;return }
  if (pivotInB === undefined) { this.ptr = _emscripten_bind_btPoint2PointConstraint_btPoint2PointConstraint_3(rbA, rbB, pivotInA); getCache(btPoint2PointConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btPoint2PointConstraint_btPoint2PointConstraint_4(rbA, rbB, pivotInA, pivotInB);
  getCache(btPoint2PointConstraint)[this.ptr] = this;
};;
btPoint2PointConstraint.prototype = Object.create(btTypedConstraint.prototype);
btPoint2PointConstraint.prototype.constructor = btPoint2PointConstraint;
btPoint2PointConstraint.prototype.__class__ = btPoint2PointConstraint;
btPoint2PointConstraint.__cache__ = {};
Module['btPoint2PointConstraint'] = btPoint2PointConstraint;

btPoint2PointConstraint.prototype['setPivotA'] = btPoint2PointConstraint.prototype.setPivotA = /** @suppress {undefinedVars, duplicate} */function(pivotA) {
  var self = this.ptr;
  if (pivotA && typeof pivotA === 'object') pivotA = pivotA.ptr;
  _emscripten_bind_btPoint2PointConstraint_setPivotA_1(self, pivotA);
};;

btPoint2PointConstraint.prototype['setPivotB'] = btPoint2PointConstraint.prototype.setPivotB = /** @suppress {undefinedVars, duplicate} */function(pivotB) {
  var self = this.ptr;
  if (pivotB && typeof pivotB === 'object') pivotB = pivotB.ptr;
  _emscripten_bind_btPoint2PointConstraint_setPivotB_1(self, pivotB);
};;

btPoint2PointConstraint.prototype['getPivotInA'] = btPoint2PointConstraint.prototype.getPivotInA = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPoint2PointConstraint_getPivotInA_0(self), btVector3);
};;

btPoint2PointConstraint.prototype['getPivotInB'] = btPoint2PointConstraint.prototype.getPivotInB = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPoint2PointConstraint_getPivotInB_0(self), btVector3);
};;

btPoint2PointConstraint.prototype['enableFeedback'] = btPoint2PointConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btPoint2PointConstraint_enableFeedback_1(self, needsFeedback);
};;

btPoint2PointConstraint.prototype['getBreakingImpulseThreshold'] = btPoint2PointConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btPoint2PointConstraint_getBreakingImpulseThreshold_0(self);
};;

btPoint2PointConstraint.prototype['setBreakingImpulseThreshold'] = btPoint2PointConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btPoint2PointConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btPoint2PointConstraint.prototype['get_m_setting'] = btPoint2PointConstraint.prototype.get_m_setting = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPoint2PointConstraint_get_m_setting_0(self), btConstraintSetting);
};
    btPoint2PointConstraint.prototype['set_m_setting'] = btPoint2PointConstraint.prototype.set_m_setting = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btPoint2PointConstraint_set_m_setting_1(self, arg0);
};
    Object.defineProperty(btPoint2PointConstraint.prototype, 'm_setting', { get: btPoint2PointConstraint.prototype.get_m_setting, set: btPoint2PointConstraint.prototype.set_m_setting });
  btPoint2PointConstraint.prototype['__destroy__'] = btPoint2PointConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btPoint2PointConstraint___destroy___0(self);
};
// btSliderConstraint
/** @suppress {undefinedVars, duplicate} */function btSliderConstraint(rbA, rbB, frameInA, frameInB, useLinearReferenceFrameA) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (frameInA && typeof frameInA === 'object') frameInA = frameInA.ptr;
  if (frameInB && typeof frameInB === 'object') frameInB = frameInB.ptr;
  if (useLinearReferenceFrameA && typeof useLinearReferenceFrameA === 'object') useLinearReferenceFrameA = useLinearReferenceFrameA.ptr;
  if (frameInB === undefined) { this.ptr = _emscripten_bind_btSliderConstraint_btSliderConstraint_3(rbA, rbB, frameInA); getCache(btSliderConstraint)[this.ptr] = this;return }
  if (useLinearReferenceFrameA === undefined) { this.ptr = _emscripten_bind_btSliderConstraint_btSliderConstraint_4(rbA, rbB, frameInA, frameInB); getCache(btSliderConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btSliderConstraint_btSliderConstraint_5(rbA, rbB, frameInA, frameInB, useLinearReferenceFrameA);
  getCache(btSliderConstraint)[this.ptr] = this;
};;
btSliderConstraint.prototype = Object.create(btTypedConstraint.prototype);
btSliderConstraint.prototype.constructor = btSliderConstraint;
btSliderConstraint.prototype.__class__ = btSliderConstraint;
btSliderConstraint.__cache__ = {};
Module['btSliderConstraint'] = btSliderConstraint;

btSliderConstraint.prototype['setLowerLinLimit'] = btSliderConstraint.prototype.setLowerLinLimit = /** @suppress {undefinedVars, duplicate} */function(lowerLimit) {
  var self = this.ptr;
  if (lowerLimit && typeof lowerLimit === 'object') lowerLimit = lowerLimit.ptr;
  _emscripten_bind_btSliderConstraint_setLowerLinLimit_1(self, lowerLimit);
};;

btSliderConstraint.prototype['setUpperLinLimit'] = btSliderConstraint.prototype.setUpperLinLimit = /** @suppress {undefinedVars, duplicate} */function(upperLimit) {
  var self = this.ptr;
  if (upperLimit && typeof upperLimit === 'object') upperLimit = upperLimit.ptr;
  _emscripten_bind_btSliderConstraint_setUpperLinLimit_1(self, upperLimit);
};;

btSliderConstraint.prototype['setLowerAngLimit'] = btSliderConstraint.prototype.setLowerAngLimit = /** @suppress {undefinedVars, duplicate} */function(lowerAngLimit) {
  var self = this.ptr;
  if (lowerAngLimit && typeof lowerAngLimit === 'object') lowerAngLimit = lowerAngLimit.ptr;
  _emscripten_bind_btSliderConstraint_setLowerAngLimit_1(self, lowerAngLimit);
};;

btSliderConstraint.prototype['setUpperAngLimit'] = btSliderConstraint.prototype.setUpperAngLimit = /** @suppress {undefinedVars, duplicate} */function(upperAngLimit) {
  var self = this.ptr;
  if (upperAngLimit && typeof upperAngLimit === 'object') upperAngLimit = upperAngLimit.ptr;
  _emscripten_bind_btSliderConstraint_setUpperAngLimit_1(self, upperAngLimit);
};;

btSliderConstraint.prototype['enableFeedback'] = btSliderConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btSliderConstraint_enableFeedback_1(self, needsFeedback);
};;

btSliderConstraint.prototype['getBreakingImpulseThreshold'] = btSliderConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btSliderConstraint_getBreakingImpulseThreshold_0(self);
};;

btSliderConstraint.prototype['setBreakingImpulseThreshold'] = btSliderConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btSliderConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btSliderConstraint.prototype['__destroy__'] = btSliderConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btSliderConstraint___destroy___0(self);
};
// btConeTwistConstraint
/** @suppress {undefinedVars, duplicate} */function btConeTwistConstraint(rbA, rbB, rbAFrame, rbBFrame) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (rbAFrame && typeof rbAFrame === 'object') rbAFrame = rbAFrame.ptr;
  if (rbBFrame && typeof rbBFrame === 'object') rbBFrame = rbBFrame.ptr;
  if (rbAFrame === undefined) { this.ptr = _emscripten_bind_btConeTwistConstraint_btConeTwistConstraint_2(rbA, rbB); getCache(btConeTwistConstraint)[this.ptr] = this;return }
  if (rbBFrame === undefined) { this.ptr = _emscripten_bind_btConeTwistConstraint_btConeTwistConstraint_3(rbA, rbB, rbAFrame); getCache(btConeTwistConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btConeTwistConstraint_btConeTwistConstraint_4(rbA, rbB, rbAFrame, rbBFrame);
  getCache(btConeTwistConstraint)[this.ptr] = this;
};;
btConeTwistConstraint.prototype = Object.create(btTypedConstraint.prototype);
btConeTwistConstraint.prototype.constructor = btConeTwistConstraint;
btConeTwistConstraint.prototype.__class__ = btConeTwistConstraint;
btConeTwistConstraint.__cache__ = {};
Module['btConeTwistConstraint'] = btConeTwistConstraint;

btConeTwistConstraint.prototype['setLimit'] = btConeTwistConstraint.prototype.setLimit = /** @suppress {undefinedVars, duplicate} */function(limitIndex, limitValue) {
  var self = this.ptr;
  if (limitIndex && typeof limitIndex === 'object') limitIndex = limitIndex.ptr;
  if (limitValue && typeof limitValue === 'object') limitValue = limitValue.ptr;
  _emscripten_bind_btConeTwistConstraint_setLimit_2(self, limitIndex, limitValue);
};;

btConeTwistConstraint.prototype['setAngularOnly'] = btConeTwistConstraint.prototype.setAngularOnly = /** @suppress {undefinedVars, duplicate} */function(angularOnly) {
  var self = this.ptr;
  if (angularOnly && typeof angularOnly === 'object') angularOnly = angularOnly.ptr;
  _emscripten_bind_btConeTwistConstraint_setAngularOnly_1(self, angularOnly);
};;

btConeTwistConstraint.prototype['setDamping'] = btConeTwistConstraint.prototype.setDamping = /** @suppress {undefinedVars, duplicate} */function(damping) {
  var self = this.ptr;
  if (damping && typeof damping === 'object') damping = damping.ptr;
  _emscripten_bind_btConeTwistConstraint_setDamping_1(self, damping);
};;

btConeTwistConstraint.prototype['enableMotor'] = btConeTwistConstraint.prototype.enableMotor = /** @suppress {undefinedVars, duplicate} */function(b) {
  var self = this.ptr;
  if (b && typeof b === 'object') b = b.ptr;
  _emscripten_bind_btConeTwistConstraint_enableMotor_1(self, b);
};;

btConeTwistConstraint.prototype['setMaxMotorImpulse'] = btConeTwistConstraint.prototype.setMaxMotorImpulse = /** @suppress {undefinedVars, duplicate} */function(maxMotorImpulse) {
  var self = this.ptr;
  if (maxMotorImpulse && typeof maxMotorImpulse === 'object') maxMotorImpulse = maxMotorImpulse.ptr;
  _emscripten_bind_btConeTwistConstraint_setMaxMotorImpulse_1(self, maxMotorImpulse);
};;

btConeTwistConstraint.prototype['setMaxMotorImpulseNormalized'] = btConeTwistConstraint.prototype.setMaxMotorImpulseNormalized = /** @suppress {undefinedVars, duplicate} */function(maxMotorImpulse) {
  var self = this.ptr;
  if (maxMotorImpulse && typeof maxMotorImpulse === 'object') maxMotorImpulse = maxMotorImpulse.ptr;
  _emscripten_bind_btConeTwistConstraint_setMaxMotorImpulseNormalized_1(self, maxMotorImpulse);
};;

btConeTwistConstraint.prototype['setMotorTarget'] = btConeTwistConstraint.prototype.setMotorTarget = /** @suppress {undefinedVars, duplicate} */function(q) {
  var self = this.ptr;
  if (q && typeof q === 'object') q = q.ptr;
  _emscripten_bind_btConeTwistConstraint_setMotorTarget_1(self, q);
};;

btConeTwistConstraint.prototype['setMotorTargetInConstraintSpace'] = btConeTwistConstraint.prototype.setMotorTargetInConstraintSpace = /** @suppress {undefinedVars, duplicate} */function(q) {
  var self = this.ptr;
  if (q && typeof q === 'object') q = q.ptr;
  _emscripten_bind_btConeTwistConstraint_setMotorTargetInConstraintSpace_1(self, q);
};;

btConeTwistConstraint.prototype['enableFeedback'] = btConeTwistConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btConeTwistConstraint_enableFeedback_1(self, needsFeedback);
};;

btConeTwistConstraint.prototype['getBreakingImpulseThreshold'] = btConeTwistConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btConeTwistConstraint_getBreakingImpulseThreshold_0(self);
};;

btConeTwistConstraint.prototype['setBreakingImpulseThreshold'] = btConeTwistConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btConeTwistConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btConeTwistConstraint.prototype['__destroy__'] = btConeTwistConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConeTwistConstraint___destroy___0(self);
};
// btDefaultCollisionConfiguration
/** @suppress {undefinedVars, duplicate} */function btDefaultCollisionConfiguration(info) {
  if (info && typeof info === 'object') info = info.ptr;
  if (info === undefined) { this.ptr = _emscripten_bind_btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_0(); getCache(btDefaultCollisionConfiguration)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_1(info);
  getCache(btDefaultCollisionConfiguration)[this.ptr] = this;
};;
btDefaultCollisionConfiguration.prototype = Object.create(WrapperObject.prototype);
btDefaultCollisionConfiguration.prototype.constructor = btDefaultCollisionConfiguration;
btDefaultCollisionConfiguration.prototype.__class__ = btDefaultCollisionConfiguration;
btDefaultCollisionConfiguration.__cache__ = {};
Module['btDefaultCollisionConfiguration'] = btDefaultCollisionConfiguration;

  btDefaultCollisionConfiguration.prototype['__destroy__'] = btDefaultCollisionConfiguration.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDefaultCollisionConfiguration___destroy___0(self);
};
// btConeShapeZ
/** @suppress {undefinedVars, duplicate} */function btConeShapeZ(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btConeShapeZ_btConeShapeZ_2(radius, height);
  getCache(btConeShapeZ)[this.ptr] = this;
};;
btConeShapeZ.prototype = Object.create(btConeShape.prototype);
btConeShapeZ.prototype.constructor = btConeShapeZ;
btConeShapeZ.prototype.__class__ = btConeShapeZ;
btConeShapeZ.__cache__ = {};
Module['btConeShapeZ'] = btConeShapeZ;

btConeShapeZ.prototype['setLocalScaling'] = btConeShapeZ.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConeShapeZ_setLocalScaling_1(self, scaling);
};;

btConeShapeZ.prototype['calculateLocalInertia'] = btConeShapeZ.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConeShapeZ_calculateLocalInertia_2(self, mass, inertia);
};;

  btConeShapeZ.prototype['__destroy__'] = btConeShapeZ.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConeShapeZ___destroy___0(self);
};
// btConeShapeX
/** @suppress {undefinedVars, duplicate} */function btConeShapeX(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btConeShapeX_btConeShapeX_2(radius, height);
  getCache(btConeShapeX)[this.ptr] = this;
};;
btConeShapeX.prototype = Object.create(btConeShape.prototype);
btConeShapeX.prototype.constructor = btConeShapeX;
btConeShapeX.prototype.__class__ = btConeShapeX;
btConeShapeX.__cache__ = {};
Module['btConeShapeX'] = btConeShapeX;

btConeShapeX.prototype['setLocalScaling'] = btConeShapeX.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConeShapeX_setLocalScaling_1(self, scaling);
};;

btConeShapeX.prototype['calculateLocalInertia'] = btConeShapeX.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConeShapeX_calculateLocalInertia_2(self, mass, inertia);
};;

  btConeShapeX.prototype['__destroy__'] = btConeShapeX.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConeShapeX___destroy___0(self);
};
// btTriangleMesh
/** @suppress {undefinedVars, duplicate} */function btTriangleMesh(use32bitIndices, use4componentVertices) {
  if (use32bitIndices && typeof use32bitIndices === 'object') use32bitIndices = use32bitIndices.ptr;
  if (use4componentVertices && typeof use4componentVertices === 'object') use4componentVertices = use4componentVertices.ptr;
  if (use32bitIndices === undefined) { this.ptr = _emscripten_bind_btTriangleMesh_btTriangleMesh_0(); getCache(btTriangleMesh)[this.ptr] = this;return }
  if (use4componentVertices === undefined) { this.ptr = _emscripten_bind_btTriangleMesh_btTriangleMesh_1(use32bitIndices); getCache(btTriangleMesh)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btTriangleMesh_btTriangleMesh_2(use32bitIndices, use4componentVertices);
  getCache(btTriangleMesh)[this.ptr] = this;
};;
btTriangleMesh.prototype = Object.create(btStridingMeshInterface.prototype);
btTriangleMesh.prototype.constructor = btTriangleMesh;
btTriangleMesh.prototype.__class__ = btTriangleMesh;
btTriangleMesh.__cache__ = {};
Module['btTriangleMesh'] = btTriangleMesh;

btTriangleMesh.prototype['addTriangle'] = btTriangleMesh.prototype.addTriangle = /** @suppress {undefinedVars, duplicate} */function(vertex0, vertex1, vertex2, removeDuplicateVertices) {
  var self = this.ptr;
  if (vertex0 && typeof vertex0 === 'object') vertex0 = vertex0.ptr;
  if (vertex1 && typeof vertex1 === 'object') vertex1 = vertex1.ptr;
  if (vertex2 && typeof vertex2 === 'object') vertex2 = vertex2.ptr;
  if (removeDuplicateVertices && typeof removeDuplicateVertices === 'object') removeDuplicateVertices = removeDuplicateVertices.ptr;
  if (removeDuplicateVertices === undefined) { _emscripten_bind_btTriangleMesh_addTriangle_3(self, vertex0, vertex1, vertex2);  return }
  _emscripten_bind_btTriangleMesh_addTriangle_4(self, vertex0, vertex1, vertex2, removeDuplicateVertices);
};;

  btTriangleMesh.prototype['__destroy__'] = btTriangleMesh.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btTriangleMesh___destroy___0(self);
};
// btConvexHullShape
/** @suppress {undefinedVars, duplicate} */function btConvexHullShape() {
  this.ptr = _emscripten_bind_btConvexHullShape_btConvexHullShape_0();
  getCache(btConvexHullShape)[this.ptr] = this;
};;
btConvexHullShape.prototype = Object.create(btCollisionShape.prototype);
btConvexHullShape.prototype.constructor = btConvexHullShape;
btConvexHullShape.prototype.__class__ = btConvexHullShape;
btConvexHullShape.__cache__ = {};
Module['btConvexHullShape'] = btConvexHullShape;

btConvexHullShape.prototype['addPoint'] = btConvexHullShape.prototype.addPoint = /** @suppress {undefinedVars, duplicate} */function(point, recalculateLocalAABB) {
  var self = this.ptr;
  if (point && typeof point === 'object') point = point.ptr;
  if (recalculateLocalAABB && typeof recalculateLocalAABB === 'object') recalculateLocalAABB = recalculateLocalAABB.ptr;
  if (recalculateLocalAABB === undefined) { _emscripten_bind_btConvexHullShape_addPoint_1(self, point);  return }
  _emscripten_bind_btConvexHullShape_addPoint_2(self, point, recalculateLocalAABB);
};;

btConvexHullShape.prototype['setLocalScaling'] = btConvexHullShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btConvexHullShape_setLocalScaling_1(self, scaling);
};;

btConvexHullShape.prototype['calculateLocalInertia'] = btConvexHullShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btConvexHullShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btConvexHullShape.prototype['__destroy__'] = btConvexHullShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConvexHullShape___destroy___0(self);
};
// btPersistentManifold
/** @suppress {undefinedVars, duplicate} */function btPersistentManifold() {
  this.ptr = _emscripten_bind_btPersistentManifold_btPersistentManifold_0();
  getCache(btPersistentManifold)[this.ptr] = this;
};;
btPersistentManifold.prototype = Object.create(WrapperObject.prototype);
btPersistentManifold.prototype.constructor = btPersistentManifold;
btPersistentManifold.prototype.__class__ = btPersistentManifold;
btPersistentManifold.__cache__ = {};
Module['btPersistentManifold'] = btPersistentManifold;

btPersistentManifold.prototype['getBody0'] = btPersistentManifold.prototype.getBody0 = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPersistentManifold_getBody0_0(self), btCollisionObject);
};;

btPersistentManifold.prototype['getBody1'] = btPersistentManifold.prototype.getBody1 = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btPersistentManifold_getBody1_0(self), btCollisionObject);
};;

btPersistentManifold.prototype['getNumContacts'] = btPersistentManifold.prototype.getNumContacts = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btPersistentManifold_getNumContacts_0(self);
};;

btPersistentManifold.prototype['getContactPoint'] = btPersistentManifold.prototype.getContactPoint = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btPersistentManifold_getContactPoint_1(self, index), btManifoldPoint);
};;

  btPersistentManifold.prototype['__destroy__'] = btPersistentManifold.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btPersistentManifold___destroy___0(self);
};
// btBoxShape
/** @suppress {undefinedVars, duplicate} */function btBoxShape(boxHalfExtents) {
  if (boxHalfExtents && typeof boxHalfExtents === 'object') boxHalfExtents = boxHalfExtents.ptr;
  this.ptr = _emscripten_bind_btBoxShape_btBoxShape_1(boxHalfExtents);
  getCache(btBoxShape)[this.ptr] = this;
};;
btBoxShape.prototype = Object.create(btCollisionShape.prototype);
btBoxShape.prototype.constructor = btBoxShape;
btBoxShape.prototype.__class__ = btBoxShape;
btBoxShape.__cache__ = {};
Module['btBoxShape'] = btBoxShape;

btBoxShape.prototype['setLocalScaling'] = btBoxShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btBoxShape_setLocalScaling_1(self, scaling);
};;

btBoxShape.prototype['calculateLocalInertia'] = btBoxShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btBoxShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btBoxShape.prototype['__destroy__'] = btBoxShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btBoxShape___destroy___0(self);
};
// btCollisionDispatcher
/** @suppress {undefinedVars, duplicate} */function btCollisionDispatcher(conf) {
  if (conf && typeof conf === 'object') conf = conf.ptr;
  this.ptr = _emscripten_bind_btCollisionDispatcher_btCollisionDispatcher_1(conf);
  getCache(btCollisionDispatcher)[this.ptr] = this;
};;
btCollisionDispatcher.prototype = Object.create(btDispatcher.prototype);
btCollisionDispatcher.prototype.constructor = btCollisionDispatcher;
btCollisionDispatcher.prototype.__class__ = btCollisionDispatcher;
btCollisionDispatcher.__cache__ = {};
Module['btCollisionDispatcher'] = btCollisionDispatcher;

btCollisionDispatcher.prototype['getNumManifolds'] = btCollisionDispatcher.prototype.getNumManifolds = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btCollisionDispatcher_getNumManifolds_0(self);
};;

btCollisionDispatcher.prototype['getManifoldByIndexInternal'] = btCollisionDispatcher.prototype.getManifoldByIndexInternal = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btCollisionDispatcher_getManifoldByIndexInternal_1(self, index), btPersistentManifold);
};;

  btCollisionDispatcher.prototype['__destroy__'] = btCollisionDispatcher.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCollisionDispatcher___destroy___0(self);
};
// btVehicleTuning
/** @suppress {undefinedVars, duplicate} */function btVehicleTuning() {
  this.ptr = _emscripten_bind_btVehicleTuning_btVehicleTuning_0();
  getCache(btVehicleTuning)[this.ptr] = this;
};;
btVehicleTuning.prototype = Object.create(WrapperObject.prototype);
btVehicleTuning.prototype.constructor = btVehicleTuning;
btVehicleTuning.prototype.__class__ = btVehicleTuning;
btVehicleTuning.__cache__ = {};
Module['btVehicleTuning'] = btVehicleTuning;

  btVehicleTuning.prototype['get_m_suspensionStiffness'] = btVehicleTuning.prototype.get_m_suspensionStiffness = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_suspensionStiffness_0(self);
};
    btVehicleTuning.prototype['set_m_suspensionStiffness'] = btVehicleTuning.prototype.set_m_suspensionStiffness = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_suspensionStiffness_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_suspensionStiffness', { get: btVehicleTuning.prototype.get_m_suspensionStiffness, set: btVehicleTuning.prototype.set_m_suspensionStiffness });
  btVehicleTuning.prototype['get_m_suspensionCompression'] = btVehicleTuning.prototype.get_m_suspensionCompression = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_suspensionCompression_0(self);
};
    btVehicleTuning.prototype['set_m_suspensionCompression'] = btVehicleTuning.prototype.set_m_suspensionCompression = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_suspensionCompression_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_suspensionCompression', { get: btVehicleTuning.prototype.get_m_suspensionCompression, set: btVehicleTuning.prototype.set_m_suspensionCompression });
  btVehicleTuning.prototype['get_m_suspensionDamping'] = btVehicleTuning.prototype.get_m_suspensionDamping = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_suspensionDamping_0(self);
};
    btVehicleTuning.prototype['set_m_suspensionDamping'] = btVehicleTuning.prototype.set_m_suspensionDamping = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_suspensionDamping_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_suspensionDamping', { get: btVehicleTuning.prototype.get_m_suspensionDamping, set: btVehicleTuning.prototype.set_m_suspensionDamping });
  btVehicleTuning.prototype['get_m_maxSuspensionTravelCm'] = btVehicleTuning.prototype.get_m_maxSuspensionTravelCm = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_maxSuspensionTravelCm_0(self);
};
    btVehicleTuning.prototype['set_m_maxSuspensionTravelCm'] = btVehicleTuning.prototype.set_m_maxSuspensionTravelCm = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_maxSuspensionTravelCm_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_maxSuspensionTravelCm', { get: btVehicleTuning.prototype.get_m_maxSuspensionTravelCm, set: btVehicleTuning.prototype.set_m_maxSuspensionTravelCm });
  btVehicleTuning.prototype['get_m_frictionSlip'] = btVehicleTuning.prototype.get_m_frictionSlip = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_frictionSlip_0(self);
};
    btVehicleTuning.prototype['set_m_frictionSlip'] = btVehicleTuning.prototype.set_m_frictionSlip = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_frictionSlip_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_frictionSlip', { get: btVehicleTuning.prototype.get_m_frictionSlip, set: btVehicleTuning.prototype.set_m_frictionSlip });
  btVehicleTuning.prototype['get_m_maxSuspensionForce'] = btVehicleTuning.prototype.get_m_maxSuspensionForce = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVehicleTuning_get_m_maxSuspensionForce_0(self);
};
    btVehicleTuning.prototype['set_m_maxSuspensionForce'] = btVehicleTuning.prototype.set_m_maxSuspensionForce = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btVehicleTuning_set_m_maxSuspensionForce_1(self, arg0);
};
    Object.defineProperty(btVehicleTuning.prototype, 'm_maxSuspensionForce', { get: btVehicleTuning.prototype.get_m_maxSuspensionForce, set: btVehicleTuning.prototype.set_m_maxSuspensionForce });
// btCompoundShape
/** @suppress {undefinedVars, duplicate} */function btCompoundShape(enableDynamicAabbTree) {
  if (enableDynamicAabbTree && typeof enableDynamicAabbTree === 'object') enableDynamicAabbTree = enableDynamicAabbTree.ptr;
  if (enableDynamicAabbTree === undefined) { this.ptr = _emscripten_bind_btCompoundShape_btCompoundShape_0(); getCache(btCompoundShape)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btCompoundShape_btCompoundShape_1(enableDynamicAabbTree);
  getCache(btCompoundShape)[this.ptr] = this;
};;
btCompoundShape.prototype = Object.create(btCollisionShape.prototype);
btCompoundShape.prototype.constructor = btCompoundShape;
btCompoundShape.prototype.__class__ = btCompoundShape;
btCompoundShape.__cache__ = {};
Module['btCompoundShape'] = btCompoundShape;

btCompoundShape.prototype['addChildShape'] = btCompoundShape.prototype.addChildShape = /** @suppress {undefinedVars, duplicate} */function(localTransform, shape) {
  var self = this.ptr;
  if (localTransform && typeof localTransform === 'object') localTransform = localTransform.ptr;
  if (shape && typeof shape === 'object') shape = shape.ptr;
  _emscripten_bind_btCompoundShape_addChildShape_2(self, localTransform, shape);
};;

btCompoundShape.prototype['setLocalScaling'] = btCompoundShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCompoundShape_setLocalScaling_1(self, scaling);
};;

btCompoundShape.prototype['calculateLocalInertia'] = btCompoundShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCompoundShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btCompoundShape.prototype['__destroy__'] = btCompoundShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCompoundShape___destroy___0(self);
};
// btCollisionObjectWrapper
/** @suppress {undefinedVars, duplicate} */function btCollisionObjectWrapper() { throw "cannot construct a btCollisionObjectWrapper, no constructor in IDL" }
btCollisionObjectWrapper.prototype = Object.create(WrapperObject.prototype);
btCollisionObjectWrapper.prototype.constructor = btCollisionObjectWrapper;
btCollisionObjectWrapper.prototype.__class__ = btCollisionObjectWrapper;
btCollisionObjectWrapper.__cache__ = {};
Module['btCollisionObjectWrapper'] = btCollisionObjectWrapper;

// ClosestConvexResultCallback
/** @suppress {undefinedVars, duplicate} */function ClosestConvexResultCallback(convexFromWorld, convexToWorld) {
  if (convexFromWorld && typeof convexFromWorld === 'object') convexFromWorld = convexFromWorld.ptr;
  if (convexToWorld && typeof convexToWorld === 'object') convexToWorld = convexToWorld.ptr;
  this.ptr = _emscripten_bind_ClosestConvexResultCallback_ClosestConvexResultCallback_2(convexFromWorld, convexToWorld);
  getCache(ClosestConvexResultCallback)[this.ptr] = this;
};;
ClosestConvexResultCallback.prototype = Object.create(ConvexResultCallback.prototype);
ClosestConvexResultCallback.prototype.constructor = ClosestConvexResultCallback;
ClosestConvexResultCallback.prototype.__class__ = ClosestConvexResultCallback;
ClosestConvexResultCallback.__cache__ = {};
Module['ClosestConvexResultCallback'] = ClosestConvexResultCallback;

ClosestConvexResultCallback.prototype['hasHit'] = ClosestConvexResultCallback.prototype.hasHit = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_ClosestConvexResultCallback_hasHit_0(self));
};;

  ClosestConvexResultCallback.prototype['get_m_convexFromWorld'] = ClosestConvexResultCallback.prototype.get_m_convexFromWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestConvexResultCallback_get_m_convexFromWorld_0(self), btVector3);
};
    ClosestConvexResultCallback.prototype['set_m_convexFromWorld'] = ClosestConvexResultCallback.prototype.set_m_convexFromWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_convexFromWorld_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_convexFromWorld', { get: ClosestConvexResultCallback.prototype.get_m_convexFromWorld, set: ClosestConvexResultCallback.prototype.set_m_convexFromWorld });
  ClosestConvexResultCallback.prototype['get_m_convexToWorld'] = ClosestConvexResultCallback.prototype.get_m_convexToWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestConvexResultCallback_get_m_convexToWorld_0(self), btVector3);
};
    ClosestConvexResultCallback.prototype['set_m_convexToWorld'] = ClosestConvexResultCallback.prototype.set_m_convexToWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_convexToWorld_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_convexToWorld', { get: ClosestConvexResultCallback.prototype.get_m_convexToWorld, set: ClosestConvexResultCallback.prototype.set_m_convexToWorld });
  ClosestConvexResultCallback.prototype['get_m_hitNormalWorld'] = ClosestConvexResultCallback.prototype.get_m_hitNormalWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestConvexResultCallback_get_m_hitNormalWorld_0(self), btVector3);
};
    ClosestConvexResultCallback.prototype['set_m_hitNormalWorld'] = ClosestConvexResultCallback.prototype.set_m_hitNormalWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_hitNormalWorld_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_hitNormalWorld', { get: ClosestConvexResultCallback.prototype.get_m_hitNormalWorld, set: ClosestConvexResultCallback.prototype.set_m_hitNormalWorld });
  ClosestConvexResultCallback.prototype['get_m_hitPointWorld'] = ClosestConvexResultCallback.prototype.get_m_hitPointWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestConvexResultCallback_get_m_hitPointWorld_0(self), btVector3);
};
    ClosestConvexResultCallback.prototype['set_m_hitPointWorld'] = ClosestConvexResultCallback.prototype.set_m_hitPointWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_hitPointWorld_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_hitPointWorld', { get: ClosestConvexResultCallback.prototype.get_m_hitPointWorld, set: ClosestConvexResultCallback.prototype.set_m_hitPointWorld });
  ClosestConvexResultCallback.prototype['get_m_collisionFilterGroup'] = ClosestConvexResultCallback.prototype.get_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ClosestConvexResultCallback_get_m_collisionFilterGroup_0(self);
};
    ClosestConvexResultCallback.prototype['set_m_collisionFilterGroup'] = ClosestConvexResultCallback.prototype.set_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_collisionFilterGroup_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_collisionFilterGroup', { get: ClosestConvexResultCallback.prototype.get_m_collisionFilterGroup, set: ClosestConvexResultCallback.prototype.set_m_collisionFilterGroup });
  ClosestConvexResultCallback.prototype['get_m_collisionFilterMask'] = ClosestConvexResultCallback.prototype.get_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ClosestConvexResultCallback_get_m_collisionFilterMask_0(self);
};
    ClosestConvexResultCallback.prototype['set_m_collisionFilterMask'] = ClosestConvexResultCallback.prototype.set_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_collisionFilterMask_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_collisionFilterMask', { get: ClosestConvexResultCallback.prototype.get_m_collisionFilterMask, set: ClosestConvexResultCallback.prototype.set_m_collisionFilterMask });
  ClosestConvexResultCallback.prototype['get_m_closestHitFraction'] = ClosestConvexResultCallback.prototype.get_m_closestHitFraction = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ClosestConvexResultCallback_get_m_closestHitFraction_0(self);
};
    ClosestConvexResultCallback.prototype['set_m_closestHitFraction'] = ClosestConvexResultCallback.prototype.set_m_closestHitFraction = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestConvexResultCallback_set_m_closestHitFraction_1(self, arg0);
};
    Object.defineProperty(ClosestConvexResultCallback.prototype, 'm_closestHitFraction', { get: ClosestConvexResultCallback.prototype.get_m_closestHitFraction, set: ClosestConvexResultCallback.prototype.set_m_closestHitFraction });
  ClosestConvexResultCallback.prototype['__destroy__'] = ClosestConvexResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_ClosestConvexResultCallback___destroy___0(self);
};
// btDefaultMotionState
/** @suppress {undefinedVars, duplicate} */function btDefaultMotionState(startTrans, centerOfMassOffset) {
  if (startTrans && typeof startTrans === 'object') startTrans = startTrans.ptr;
  if (centerOfMassOffset && typeof centerOfMassOffset === 'object') centerOfMassOffset = centerOfMassOffset.ptr;
  if (startTrans === undefined) { this.ptr = _emscripten_bind_btDefaultMotionState_btDefaultMotionState_0(); getCache(btDefaultMotionState)[this.ptr] = this;return }
  if (centerOfMassOffset === undefined) { this.ptr = _emscripten_bind_btDefaultMotionState_btDefaultMotionState_1(startTrans); getCache(btDefaultMotionState)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btDefaultMotionState_btDefaultMotionState_2(startTrans, centerOfMassOffset);
  getCache(btDefaultMotionState)[this.ptr] = this;
};;
btDefaultMotionState.prototype = Object.create(btMotionState.prototype);
btDefaultMotionState.prototype.constructor = btDefaultMotionState;
btDefaultMotionState.prototype.__class__ = btDefaultMotionState;
btDefaultMotionState.__cache__ = {};
Module['btDefaultMotionState'] = btDefaultMotionState;

btDefaultMotionState.prototype['getWorldTransform'] = btDefaultMotionState.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btDefaultMotionState_getWorldTransform_1(self, worldTrans);
};;

btDefaultMotionState.prototype['setWorldTransform'] = btDefaultMotionState.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btDefaultMotionState_setWorldTransform_1(self, worldTrans);
};;

  btDefaultMotionState.prototype['get_m_graphicsWorldTrans'] = btDefaultMotionState.prototype.get_m_graphicsWorldTrans = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btDefaultMotionState_get_m_graphicsWorldTrans_0(self), btTransform);
};
    btDefaultMotionState.prototype['set_m_graphicsWorldTrans'] = btDefaultMotionState.prototype.set_m_graphicsWorldTrans = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btDefaultMotionState_set_m_graphicsWorldTrans_1(self, arg0);
};
    Object.defineProperty(btDefaultMotionState.prototype, 'm_graphicsWorldTrans', { get: btDefaultMotionState.prototype.get_m_graphicsWorldTrans, set: btDefaultMotionState.prototype.set_m_graphicsWorldTrans });
  btDefaultMotionState.prototype['__destroy__'] = btDefaultMotionState.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDefaultMotionState___destroy___0(self);
};
// ClosestRayResultCallback
/** @suppress {undefinedVars, duplicate} */function ClosestRayResultCallback(from, to) {
  if (from && typeof from === 'object') from = from.ptr;
  if (to && typeof to === 'object') to = to.ptr;
  this.ptr = _emscripten_bind_ClosestRayResultCallback_ClosestRayResultCallback_2(from, to);
  getCache(ClosestRayResultCallback)[this.ptr] = this;
};;
ClosestRayResultCallback.prototype = Object.create(RayResultCallback.prototype);
ClosestRayResultCallback.prototype.constructor = ClosestRayResultCallback;
ClosestRayResultCallback.prototype.__class__ = ClosestRayResultCallback;
ClosestRayResultCallback.__cache__ = {};
Module['ClosestRayResultCallback'] = ClosestRayResultCallback;

ClosestRayResultCallback.prototype['hasHit'] = ClosestRayResultCallback.prototype.hasHit = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_ClosestRayResultCallback_hasHit_0(self));
};;

  ClosestRayResultCallback.prototype['get_m_rayFromWorld'] = ClosestRayResultCallback.prototype.get_m_rayFromWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestRayResultCallback_get_m_rayFromWorld_0(self), btVector3);
};
    ClosestRayResultCallback.prototype['set_m_rayFromWorld'] = ClosestRayResultCallback.prototype.set_m_rayFromWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_rayFromWorld_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_rayFromWorld', { get: ClosestRayResultCallback.prototype.get_m_rayFromWorld, set: ClosestRayResultCallback.prototype.set_m_rayFromWorld });
  ClosestRayResultCallback.prototype['get_m_rayToWorld'] = ClosestRayResultCallback.prototype.get_m_rayToWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestRayResultCallback_get_m_rayToWorld_0(self), btVector3);
};
    ClosestRayResultCallback.prototype['set_m_rayToWorld'] = ClosestRayResultCallback.prototype.set_m_rayToWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_rayToWorld_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_rayToWorld', { get: ClosestRayResultCallback.prototype.get_m_rayToWorld, set: ClosestRayResultCallback.prototype.set_m_rayToWorld });
  ClosestRayResultCallback.prototype['get_m_hitNormalWorld'] = ClosestRayResultCallback.prototype.get_m_hitNormalWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestRayResultCallback_get_m_hitNormalWorld_0(self), btVector3);
};
    ClosestRayResultCallback.prototype['set_m_hitNormalWorld'] = ClosestRayResultCallback.prototype.set_m_hitNormalWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_hitNormalWorld_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_hitNormalWorld', { get: ClosestRayResultCallback.prototype.get_m_hitNormalWorld, set: ClosestRayResultCallback.prototype.set_m_hitNormalWorld });
  ClosestRayResultCallback.prototype['get_m_hitPointWorld'] = ClosestRayResultCallback.prototype.get_m_hitPointWorld = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestRayResultCallback_get_m_hitPointWorld_0(self), btVector3);
};
    ClosestRayResultCallback.prototype['set_m_hitPointWorld'] = ClosestRayResultCallback.prototype.set_m_hitPointWorld = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_hitPointWorld_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_hitPointWorld', { get: ClosestRayResultCallback.prototype.get_m_hitPointWorld, set: ClosestRayResultCallback.prototype.set_m_hitPointWorld });
  ClosestRayResultCallback.prototype['get_m_collisionFilterGroup'] = ClosestRayResultCallback.prototype.get_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ClosestRayResultCallback_get_m_collisionFilterGroup_0(self);
};
    ClosestRayResultCallback.prototype['set_m_collisionFilterGroup'] = ClosestRayResultCallback.prototype.set_m_collisionFilterGroup = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_collisionFilterGroup_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_collisionFilterGroup', { get: ClosestRayResultCallback.prototype.get_m_collisionFilterGroup, set: ClosestRayResultCallback.prototype.set_m_collisionFilterGroup });
  ClosestRayResultCallback.prototype['get_m_collisionFilterMask'] = ClosestRayResultCallback.prototype.get_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_ClosestRayResultCallback_get_m_collisionFilterMask_0(self);
};
    ClosestRayResultCallback.prototype['set_m_collisionFilterMask'] = ClosestRayResultCallback.prototype.set_m_collisionFilterMask = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_collisionFilterMask_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_collisionFilterMask', { get: ClosestRayResultCallback.prototype.get_m_collisionFilterMask, set: ClosestRayResultCallback.prototype.set_m_collisionFilterMask });
  ClosestRayResultCallback.prototype['get_m_collisionObject'] = ClosestRayResultCallback.prototype.get_m_collisionObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ClosestRayResultCallback_get_m_collisionObject_0(self), btCollisionObject);
};
    ClosestRayResultCallback.prototype['set_m_collisionObject'] = ClosestRayResultCallback.prototype.set_m_collisionObject = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_ClosestRayResultCallback_set_m_collisionObject_1(self, arg0);
};
    Object.defineProperty(ClosestRayResultCallback.prototype, 'm_collisionObject', { get: ClosestRayResultCallback.prototype.get_m_collisionObject, set: ClosestRayResultCallback.prototype.set_m_collisionObject });
  ClosestRayResultCallback.prototype['__destroy__'] = ClosestRayResultCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_ClosestRayResultCallback___destroy___0(self);
};
// btCapsuleShapeX
/** @suppress {undefinedVars, duplicate} */function btCapsuleShapeX(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btCapsuleShapeX_btCapsuleShapeX_2(radius, height);
  getCache(btCapsuleShapeX)[this.ptr] = this;
};;
btCapsuleShapeX.prototype = Object.create(btCapsuleShape.prototype);
btCapsuleShapeX.prototype.constructor = btCapsuleShapeX;
btCapsuleShapeX.prototype.__class__ = btCapsuleShapeX;
btCapsuleShapeX.__cache__ = {};
Module['btCapsuleShapeX'] = btCapsuleShapeX;

btCapsuleShapeX.prototype['setLocalScaling'] = btCapsuleShapeX.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCapsuleShapeX_setLocalScaling_1(self, scaling);
};;

btCapsuleShapeX.prototype['calculateLocalInertia'] = btCapsuleShapeX.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCapsuleShapeX_calculateLocalInertia_2(self, mass, inertia);
};;

  btCapsuleShapeX.prototype['__destroy__'] = btCapsuleShapeX.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCapsuleShapeX___destroy___0(self);
};
// btQuaternion
/** @suppress {undefinedVars, duplicate} */function btQuaternion(x, y, z, w) {
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  this.ptr = _emscripten_bind_btQuaternion_btQuaternion_4(x, y, z, w);
  getCache(btQuaternion)[this.ptr] = this;
};;
btQuaternion.prototype = Object.create(btQuadWord.prototype);
btQuaternion.prototype.constructor = btQuaternion;
btQuaternion.prototype.__class__ = btQuaternion;
btQuaternion.__cache__ = {};
Module['btQuaternion'] = btQuaternion;

btQuaternion.prototype['setValue'] = btQuaternion.prototype.setValue = /** @suppress {undefinedVars, duplicate} */function(x, y, z, w) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  _emscripten_bind_btQuaternion_setValue_4(self, x, y, z, w);
};;

btQuaternion.prototype['x'] = btQuaternion.prototype.x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuaternion_x_0(self);
};;

btQuaternion.prototype['y'] = btQuaternion.prototype.y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuaternion_y_0(self);
};;

btQuaternion.prototype['z'] = btQuaternion.prototype.z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuaternion_z_0(self);
};;

btQuaternion.prototype['w'] = btQuaternion.prototype.w = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btQuaternion_w_0(self);
};;

btQuaternion.prototype['setX'] = btQuaternion.prototype.setX = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_btQuaternion_setX_1(self, x);
};;

btQuaternion.prototype['setY'] = btQuaternion.prototype.setY = /** @suppress {undefinedVars, duplicate} */function(y) {
  var self = this.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  _emscripten_bind_btQuaternion_setY_1(self, y);
};;

btQuaternion.prototype['setZ'] = btQuaternion.prototype.setZ = /** @suppress {undefinedVars, duplicate} */function(z) {
  var self = this.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_btQuaternion_setZ_1(self, z);
};;

btQuaternion.prototype['setW'] = btQuaternion.prototype.setW = /** @suppress {undefinedVars, duplicate} */function(w) {
  var self = this.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  _emscripten_bind_btQuaternion_setW_1(self, w);
};;

  btQuaternion.prototype['__destroy__'] = btQuaternion.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btQuaternion___destroy___0(self);
};
// btCapsuleShapeZ
/** @suppress {undefinedVars, duplicate} */function btCapsuleShapeZ(radius, height) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  if (height && typeof height === 'object') height = height.ptr;
  this.ptr = _emscripten_bind_btCapsuleShapeZ_btCapsuleShapeZ_2(radius, height);
  getCache(btCapsuleShapeZ)[this.ptr] = this;
};;
btCapsuleShapeZ.prototype = Object.create(btCapsuleShape.prototype);
btCapsuleShapeZ.prototype.constructor = btCapsuleShapeZ;
btCapsuleShapeZ.prototype.__class__ = btCapsuleShapeZ;
btCapsuleShapeZ.__cache__ = {};
Module['btCapsuleShapeZ'] = btCapsuleShapeZ;

btCapsuleShapeZ.prototype['setLocalScaling'] = btCapsuleShapeZ.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCapsuleShapeZ_setLocalScaling_1(self, scaling);
};;

btCapsuleShapeZ.prototype['calculateLocalInertia'] = btCapsuleShapeZ.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCapsuleShapeZ_calculateLocalInertia_2(self, mass, inertia);
};;

  btCapsuleShapeZ.prototype['__destroy__'] = btCapsuleShapeZ.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCapsuleShapeZ___destroy___0(self);
};
// btDefaultVehicleRaycaster
/** @suppress {undefinedVars, duplicate} */function btDefaultVehicleRaycaster(world) {
  if (world && typeof world === 'object') world = world.ptr;
  this.ptr = _emscripten_bind_btDefaultVehicleRaycaster_btDefaultVehicleRaycaster_1(world);
  getCache(btDefaultVehicleRaycaster)[this.ptr] = this;
};;
btDefaultVehicleRaycaster.prototype = Object.create(btVehicleRaycaster.prototype);
btDefaultVehicleRaycaster.prototype.constructor = btDefaultVehicleRaycaster;
btDefaultVehicleRaycaster.prototype.__class__ = btDefaultVehicleRaycaster;
btDefaultVehicleRaycaster.__cache__ = {};
Module['btDefaultVehicleRaycaster'] = btDefaultVehicleRaycaster;

  btDefaultVehicleRaycaster.prototype['__destroy__'] = btDefaultVehicleRaycaster.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDefaultVehicleRaycaster___destroy___0(self);
};
// btWheelInfo
/** @suppress {undefinedVars, duplicate} */function btWheelInfo() { throw "cannot construct a btWheelInfo, no constructor in IDL" }
btWheelInfo.prototype = Object.create(WrapperObject.prototype);
btWheelInfo.prototype.constructor = btWheelInfo;
btWheelInfo.prototype.__class__ = btWheelInfo;
btWheelInfo.__cache__ = {};
Module['btWheelInfo'] = btWheelInfo;

  btWheelInfo.prototype['get_m_suspensionStiffness'] = btWheelInfo.prototype.get_m_suspensionStiffness = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_suspensionStiffness_0(self);
};
    btWheelInfo.prototype['set_m_suspensionStiffness'] = btWheelInfo.prototype.set_m_suspensionStiffness = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_suspensionStiffness_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_suspensionStiffness', { get: btWheelInfo.prototype.get_m_suspensionStiffness, set: btWheelInfo.prototype.set_m_suspensionStiffness });
  btWheelInfo.prototype['get_m_frictionSlip'] = btWheelInfo.prototype.get_m_frictionSlip = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_frictionSlip_0(self);
};
    btWheelInfo.prototype['set_m_frictionSlip'] = btWheelInfo.prototype.set_m_frictionSlip = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_frictionSlip_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_frictionSlip', { get: btWheelInfo.prototype.get_m_frictionSlip, set: btWheelInfo.prototype.set_m_frictionSlip });
  btWheelInfo.prototype['get_m_rollInfluence'] = btWheelInfo.prototype.get_m_rollInfluence = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_rollInfluence_0(self);
};
    btWheelInfo.prototype['set_m_rollInfluence'] = btWheelInfo.prototype.set_m_rollInfluence = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_rollInfluence_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_rollInfluence', { get: btWheelInfo.prototype.get_m_rollInfluence, set: btWheelInfo.prototype.set_m_rollInfluence });
  btWheelInfo.prototype['get_m_suspensionRestLength1'] = btWheelInfo.prototype.get_m_suspensionRestLength1 = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_suspensionRestLength1_0(self);
};
    btWheelInfo.prototype['set_m_suspensionRestLength1'] = btWheelInfo.prototype.set_m_suspensionRestLength1 = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_suspensionRestLength1_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_suspensionRestLength1', { get: btWheelInfo.prototype.get_m_suspensionRestLength1, set: btWheelInfo.prototype.set_m_suspensionRestLength1 });
  btWheelInfo.prototype['get_m_wheelsRadius'] = btWheelInfo.prototype.get_m_wheelsRadius = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_wheelsRadius_0(self);
};
    btWheelInfo.prototype['set_m_wheelsRadius'] = btWheelInfo.prototype.set_m_wheelsRadius = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_wheelsRadius_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_wheelsRadius', { get: btWheelInfo.prototype.get_m_wheelsRadius, set: btWheelInfo.prototype.set_m_wheelsRadius });
  btWheelInfo.prototype['get_m_wheelsDampingCompression'] = btWheelInfo.prototype.get_m_wheelsDampingCompression = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_wheelsDampingCompression_0(self);
};
    btWheelInfo.prototype['set_m_wheelsDampingCompression'] = btWheelInfo.prototype.set_m_wheelsDampingCompression = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_wheelsDampingCompression_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_wheelsDampingCompression', { get: btWheelInfo.prototype.get_m_wheelsDampingCompression, set: btWheelInfo.prototype.set_m_wheelsDampingCompression });
  btWheelInfo.prototype['get_m_wheelsDampingRelaxation'] = btWheelInfo.prototype.get_m_wheelsDampingRelaxation = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_wheelsDampingRelaxation_0(self);
};
    btWheelInfo.prototype['set_m_wheelsDampingRelaxation'] = btWheelInfo.prototype.set_m_wheelsDampingRelaxation = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_wheelsDampingRelaxation_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_wheelsDampingRelaxation', { get: btWheelInfo.prototype.get_m_wheelsDampingRelaxation, set: btWheelInfo.prototype.set_m_wheelsDampingRelaxation });
  btWheelInfo.prototype['get_m_steering'] = btWheelInfo.prototype.get_m_steering = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_steering_0(self);
};
    btWheelInfo.prototype['set_m_steering'] = btWheelInfo.prototype.set_m_steering = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_steering_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_steering', { get: btWheelInfo.prototype.get_m_steering, set: btWheelInfo.prototype.set_m_steering });
  btWheelInfo.prototype['get_m_maxSuspensionForce'] = btWheelInfo.prototype.get_m_maxSuspensionForce = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_maxSuspensionForce_0(self);
};
    btWheelInfo.prototype['set_m_maxSuspensionForce'] = btWheelInfo.prototype.set_m_maxSuspensionForce = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_maxSuspensionForce_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_maxSuspensionForce', { get: btWheelInfo.prototype.get_m_maxSuspensionForce, set: btWheelInfo.prototype.set_m_maxSuspensionForce });
  btWheelInfo.prototype['get_m_maxSuspensionTravelCm'] = btWheelInfo.prototype.get_m_maxSuspensionTravelCm = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_maxSuspensionTravelCm_0(self);
};
    btWheelInfo.prototype['set_m_maxSuspensionTravelCm'] = btWheelInfo.prototype.set_m_maxSuspensionTravelCm = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_maxSuspensionTravelCm_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_maxSuspensionTravelCm', { get: btWheelInfo.prototype.get_m_maxSuspensionTravelCm, set: btWheelInfo.prototype.set_m_maxSuspensionTravelCm });
  btWheelInfo.prototype['get_m_wheelsSuspensionForce'] = btWheelInfo.prototype.get_m_wheelsSuspensionForce = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btWheelInfo_get_m_wheelsSuspensionForce_0(self);
};
    btWheelInfo.prototype['set_m_wheelsSuspensionForce'] = btWheelInfo.prototype.set_m_wheelsSuspensionForce = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_wheelsSuspensionForce_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_wheelsSuspensionForce', { get: btWheelInfo.prototype.get_m_wheelsSuspensionForce, set: btWheelInfo.prototype.set_m_wheelsSuspensionForce });
  btWheelInfo.prototype['get_m_bIsFrontWheel'] = btWheelInfo.prototype.get_m_bIsFrontWheel = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btWheelInfo_get_m_bIsFrontWheel_0(self));
};
    btWheelInfo.prototype['set_m_bIsFrontWheel'] = btWheelInfo.prototype.set_m_bIsFrontWheel = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_bIsFrontWheel_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_bIsFrontWheel', { get: btWheelInfo.prototype.get_m_bIsFrontWheel, set: btWheelInfo.prototype.set_m_bIsFrontWheel });
  btWheelInfo.prototype['get_m_chassisConnectionPointCS'] = btWheelInfo.prototype.get_m_chassisConnectionPointCS = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btWheelInfo_get_m_chassisConnectionPointCS_0(self), btVector3);
};
    btWheelInfo.prototype['set_m_chassisConnectionPointCS'] = btWheelInfo.prototype.set_m_chassisConnectionPointCS = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btWheelInfo_set_m_chassisConnectionPointCS_1(self, arg0);
};
    Object.defineProperty(btWheelInfo.prototype, 'm_chassisConnectionPointCS', { get: btWheelInfo.prototype.get_m_chassisConnectionPointCS, set: btWheelInfo.prototype.set_m_chassisConnectionPointCS });
  btWheelInfo.prototype['__destroy__'] = btWheelInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btWheelInfo___destroy___0(self);
};
// btVector4
/** @suppress {undefinedVars, duplicate} */function btVector4(x, y, z, w) {
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  if (x === undefined) { this.ptr = _emscripten_bind_btVector4_btVector4_0(); getCache(btVector4)[this.ptr] = this;return }
  if (y === undefined) { this.ptr = _emscripten_bind_btVector4_btVector4_1(x); getCache(btVector4)[this.ptr] = this;return }
  if (z === undefined) { this.ptr = _emscripten_bind_btVector4_btVector4_2(x, y); getCache(btVector4)[this.ptr] = this;return }
  if (w === undefined) { this.ptr = _emscripten_bind_btVector4_btVector4_3(x, y, z); getCache(btVector4)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btVector4_btVector4_4(x, y, z, w);
  getCache(btVector4)[this.ptr] = this;
};;
btVector4.prototype = Object.create(btVector3.prototype);
btVector4.prototype.constructor = btVector4;
btVector4.prototype.__class__ = btVector4;
btVector4.__cache__ = {};
Module['btVector4'] = btVector4;

btVector4.prototype['w'] = btVector4.prototype.w = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector4_w_0(self);
};;

btVector4.prototype['setValue'] = btVector4.prototype.setValue = /** @suppress {undefinedVars, duplicate} */function(x, y, z, w) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  if (w && typeof w === 'object') w = w.ptr;
  _emscripten_bind_btVector4_setValue_4(self, x, y, z, w);
};;

btVector4.prototype['length'] = btVector4.prototype.length = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector4_length_0(self);
};;

btVector4.prototype['x'] = btVector4.prototype.x = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector4_x_0(self);
};;

btVector4.prototype['y'] = btVector4.prototype.y = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector4_y_0(self);
};;

btVector4.prototype['z'] = btVector4.prototype.z = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btVector4_z_0(self);
};;

btVector4.prototype['setX'] = btVector4.prototype.setX = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_btVector4_setX_1(self, x);
};;

btVector4.prototype['setY'] = btVector4.prototype.setY = /** @suppress {undefinedVars, duplicate} */function(y) {
  var self = this.ptr;
  if (y && typeof y === 'object') y = y.ptr;
  _emscripten_bind_btVector4_setY_1(self, y);
};;

btVector4.prototype['setZ'] = btVector4.prototype.setZ = /** @suppress {undefinedVars, duplicate} */function(z) {
  var self = this.ptr;
  if (z && typeof z === 'object') z = z.ptr;
  _emscripten_bind_btVector4_setZ_1(self, z);
};;

btVector4.prototype['op_mul'] = btVector4.prototype.op_mul = /** @suppress {undefinedVars, duplicate} */function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  return wrapPointer(_emscripten_bind_btVector4_op_mul_1(self, x), btVector3);
};;

  btVector4.prototype['__destroy__'] = btVector4.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btVector4___destroy___0(self);
};
// btKinematicCharacterController
/** @suppress {undefinedVars, duplicate} */function btKinematicCharacterController(ghostObject, convexShape, stepHeight, upAxis) {
  if (ghostObject && typeof ghostObject === 'object') ghostObject = ghostObject.ptr;
  if (convexShape && typeof convexShape === 'object') convexShape = convexShape.ptr;
  if (stepHeight && typeof stepHeight === 'object') stepHeight = stepHeight.ptr;
  if (upAxis && typeof upAxis === 'object') upAxis = upAxis.ptr;
  if (upAxis === undefined) { this.ptr = _emscripten_bind_btKinematicCharacterController_btKinematicCharacterController_3(ghostObject, convexShape, stepHeight); getCache(btKinematicCharacterController)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btKinematicCharacterController_btKinematicCharacterController_4(ghostObject, convexShape, stepHeight, upAxis);
  getCache(btKinematicCharacterController)[this.ptr] = this;
};;
btKinematicCharacterController.prototype = Object.create(btActionInterface.prototype);
btKinematicCharacterController.prototype.constructor = btKinematicCharacterController;
btKinematicCharacterController.prototype.__class__ = btKinematicCharacterController;
btKinematicCharacterController.__cache__ = {};
Module['btKinematicCharacterController'] = btKinematicCharacterController;

btKinematicCharacterController.prototype['setUpAxis'] = btKinematicCharacterController.prototype.setUpAxis = /** @suppress {undefinedVars, duplicate} */function(axis) {
  var self = this.ptr;
  if (axis && typeof axis === 'object') axis = axis.ptr;
  _emscripten_bind_btKinematicCharacterController_setUpAxis_1(self, axis);
};;

btKinematicCharacterController.prototype['setWalkDirection'] = btKinematicCharacterController.prototype.setWalkDirection = /** @suppress {undefinedVars, duplicate} */function(walkDirection) {
  var self = this.ptr;
  if (walkDirection && typeof walkDirection === 'object') walkDirection = walkDirection.ptr;
  _emscripten_bind_btKinematicCharacterController_setWalkDirection_1(self, walkDirection);
};;

btKinematicCharacterController.prototype['setVelocityForTimeInterval'] = btKinematicCharacterController.prototype.setVelocityForTimeInterval = /** @suppress {undefinedVars, duplicate} */function(velocity, timeInterval) {
  var self = this.ptr;
  if (velocity && typeof velocity === 'object') velocity = velocity.ptr;
  if (timeInterval && typeof timeInterval === 'object') timeInterval = timeInterval.ptr;
  _emscripten_bind_btKinematicCharacterController_setVelocityForTimeInterval_2(self, velocity, timeInterval);
};;

btKinematicCharacterController.prototype['warp'] = btKinematicCharacterController.prototype.warp = /** @suppress {undefinedVars, duplicate} */function(origin) {
  var self = this.ptr;
  if (origin && typeof origin === 'object') origin = origin.ptr;
  _emscripten_bind_btKinematicCharacterController_warp_1(self, origin);
};;

btKinematicCharacterController.prototype['preStep'] = btKinematicCharacterController.prototype.preStep = /** @suppress {undefinedVars, duplicate} */function(collisionWorld) {
  var self = this.ptr;
  if (collisionWorld && typeof collisionWorld === 'object') collisionWorld = collisionWorld.ptr;
  _emscripten_bind_btKinematicCharacterController_preStep_1(self, collisionWorld);
};;

btKinematicCharacterController.prototype['playerStep'] = btKinematicCharacterController.prototype.playerStep = /** @suppress {undefinedVars, duplicate} */function(collisionWorld, dt) {
  var self = this.ptr;
  if (collisionWorld && typeof collisionWorld === 'object') collisionWorld = collisionWorld.ptr;
  if (dt && typeof dt === 'object') dt = dt.ptr;
  _emscripten_bind_btKinematicCharacterController_playerStep_2(self, collisionWorld, dt);
};;

btKinematicCharacterController.prototype['setFallSpeed'] = btKinematicCharacterController.prototype.setFallSpeed = /** @suppress {undefinedVars, duplicate} */function(fallSpeed) {
  var self = this.ptr;
  if (fallSpeed && typeof fallSpeed === 'object') fallSpeed = fallSpeed.ptr;
  _emscripten_bind_btKinematicCharacterController_setFallSpeed_1(self, fallSpeed);
};;

btKinematicCharacterController.prototype['setJumpSpeed'] = btKinematicCharacterController.prototype.setJumpSpeed = /** @suppress {undefinedVars, duplicate} */function(jumpSpeed) {
  var self = this.ptr;
  if (jumpSpeed && typeof jumpSpeed === 'object') jumpSpeed = jumpSpeed.ptr;
  _emscripten_bind_btKinematicCharacterController_setJumpSpeed_1(self, jumpSpeed);
};;

btKinematicCharacterController.prototype['setMaxJumpHeight'] = btKinematicCharacterController.prototype.setMaxJumpHeight = /** @suppress {undefinedVars, duplicate} */function(maxJumpHeight) {
  var self = this.ptr;
  if (maxJumpHeight && typeof maxJumpHeight === 'object') maxJumpHeight = maxJumpHeight.ptr;
  _emscripten_bind_btKinematicCharacterController_setMaxJumpHeight_1(self, maxJumpHeight);
};;

btKinematicCharacterController.prototype['canJump'] = btKinematicCharacterController.prototype.canJump = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btKinematicCharacterController_canJump_0(self));
};;

btKinematicCharacterController.prototype['jump'] = btKinematicCharacterController.prototype.jump = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btKinematicCharacterController_jump_0(self);
};;

btKinematicCharacterController.prototype['setGravity'] = btKinematicCharacterController.prototype.setGravity = /** @suppress {undefinedVars, duplicate} */function(gravity) {
  var self = this.ptr;
  if (gravity && typeof gravity === 'object') gravity = gravity.ptr;
  _emscripten_bind_btKinematicCharacterController_setGravity_1(self, gravity);
};;

btKinematicCharacterController.prototype['getGravity'] = btKinematicCharacterController.prototype.getGravity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btKinematicCharacterController_getGravity_0(self);
};;

btKinematicCharacterController.prototype['setMaxSlope'] = btKinematicCharacterController.prototype.setMaxSlope = /** @suppress {undefinedVars, duplicate} */function(slopeRadians) {
  var self = this.ptr;
  if (slopeRadians && typeof slopeRadians === 'object') slopeRadians = slopeRadians.ptr;
  _emscripten_bind_btKinematicCharacterController_setMaxSlope_1(self, slopeRadians);
};;

btKinematicCharacterController.prototype['getMaxSlope'] = btKinematicCharacterController.prototype.getMaxSlope = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btKinematicCharacterController_getMaxSlope_0(self);
};;

btKinematicCharacterController.prototype['getGhostObject'] = btKinematicCharacterController.prototype.getGhostObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btKinematicCharacterController_getGhostObject_0(self), btPairCachingGhostObject);
};;

btKinematicCharacterController.prototype['setUseGhostSweepTest'] = btKinematicCharacterController.prototype.setUseGhostSweepTest = /** @suppress {undefinedVars, duplicate} */function(useGhostObjectSweepTest) {
  var self = this.ptr;
  if (useGhostObjectSweepTest && typeof useGhostObjectSweepTest === 'object') useGhostObjectSweepTest = useGhostObjectSweepTest.ptr;
  _emscripten_bind_btKinematicCharacterController_setUseGhostSweepTest_1(self, useGhostObjectSweepTest);
};;

btKinematicCharacterController.prototype['onGround'] = btKinematicCharacterController.prototype.onGround = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btKinematicCharacterController_onGround_0(self));
};;

  btKinematicCharacterController.prototype['__destroy__'] = btKinematicCharacterController.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btKinematicCharacterController___destroy___0(self);
};
// btStaticPlaneShape
/** @suppress {undefinedVars, duplicate} */function btStaticPlaneShape(planeNormal, planeConstant) {
  if (planeNormal && typeof planeNormal === 'object') planeNormal = planeNormal.ptr;
  if (planeConstant && typeof planeConstant === 'object') planeConstant = planeConstant.ptr;
  this.ptr = _emscripten_bind_btStaticPlaneShape_btStaticPlaneShape_2(planeNormal, planeConstant);
  getCache(btStaticPlaneShape)[this.ptr] = this;
};;
btStaticPlaneShape.prototype = Object.create(btConcaveShape.prototype);
btStaticPlaneShape.prototype.constructor = btStaticPlaneShape;
btStaticPlaneShape.prototype.__class__ = btStaticPlaneShape;
btStaticPlaneShape.__cache__ = {};
Module['btStaticPlaneShape'] = btStaticPlaneShape;

btStaticPlaneShape.prototype['setLocalScaling'] = btStaticPlaneShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btStaticPlaneShape_setLocalScaling_1(self, scaling);
};;

btStaticPlaneShape.prototype['calculateLocalInertia'] = btStaticPlaneShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btStaticPlaneShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btStaticPlaneShape.prototype['__destroy__'] = btStaticPlaneShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btStaticPlaneShape___destroy___0(self);
};
// btContactSolverInfo
/** @suppress {undefinedVars, duplicate} */function btContactSolverInfo() { throw "cannot construct a btContactSolverInfo, no constructor in IDL" }
btContactSolverInfo.prototype = Object.create(WrapperObject.prototype);
btContactSolverInfo.prototype.constructor = btContactSolverInfo;
btContactSolverInfo.prototype.__class__ = btContactSolverInfo;
btContactSolverInfo.__cache__ = {};
Module['btContactSolverInfo'] = btContactSolverInfo;

  btContactSolverInfo.prototype['get_m_splitImpulse'] = btContactSolverInfo.prototype.get_m_splitImpulse = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btContactSolverInfo_get_m_splitImpulse_0(self));
};
    btContactSolverInfo.prototype['set_m_splitImpulse'] = btContactSolverInfo.prototype.set_m_splitImpulse = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btContactSolverInfo_set_m_splitImpulse_1(self, arg0);
};
    Object.defineProperty(btContactSolverInfo.prototype, 'm_splitImpulse', { get: btContactSolverInfo.prototype.get_m_splitImpulse, set: btContactSolverInfo.prototype.set_m_splitImpulse });
  btContactSolverInfo.prototype['get_m_splitImpulsePenetrationThreshold'] = btContactSolverInfo.prototype.get_m_splitImpulsePenetrationThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btContactSolverInfo_get_m_splitImpulsePenetrationThreshold_0(self);
};
    btContactSolverInfo.prototype['set_m_splitImpulsePenetrationThreshold'] = btContactSolverInfo.prototype.set_m_splitImpulsePenetrationThreshold = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btContactSolverInfo_set_m_splitImpulsePenetrationThreshold_1(self, arg0);
};
    Object.defineProperty(btContactSolverInfo.prototype, 'm_splitImpulsePenetrationThreshold', { get: btContactSolverInfo.prototype.get_m_splitImpulsePenetrationThreshold, set: btContactSolverInfo.prototype.set_m_splitImpulsePenetrationThreshold });
  btContactSolverInfo.prototype['__destroy__'] = btContactSolverInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btContactSolverInfo___destroy___0(self);
};
// btOverlappingPairCache
/** @suppress {undefinedVars, duplicate} */function btOverlappingPairCache() { throw "cannot construct a btOverlappingPairCache, no constructor in IDL" }
btOverlappingPairCache.prototype = Object.create(WrapperObject.prototype);
btOverlappingPairCache.prototype.constructor = btOverlappingPairCache;
btOverlappingPairCache.prototype.__class__ = btOverlappingPairCache;
btOverlappingPairCache.__cache__ = {};
Module['btOverlappingPairCache'] = btOverlappingPairCache;

btOverlappingPairCache.prototype['setInternalGhostPairCallback'] = btOverlappingPairCache.prototype.setInternalGhostPairCallback = /** @suppress {undefinedVars, duplicate} */function(ghostPairCallback) {
  var self = this.ptr;
  if (ghostPairCallback && typeof ghostPairCallback === 'object') ghostPairCallback = ghostPairCallback.ptr;
  _emscripten_bind_btOverlappingPairCache_setInternalGhostPairCallback_1(self, ghostPairCallback);
};;

  btOverlappingPairCache.prototype['__destroy__'] = btOverlappingPairCache.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btOverlappingPairCache___destroy___0(self);
};
// btSphereShape
/** @suppress {undefinedVars, duplicate} */function btSphereShape(radius) {
  if (radius && typeof radius === 'object') radius = radius.ptr;
  this.ptr = _emscripten_bind_btSphereShape_btSphereShape_1(radius);
  getCache(btSphereShape)[this.ptr] = this;
};;
btSphereShape.prototype = Object.create(btCollisionShape.prototype);
btSphereShape.prototype.constructor = btSphereShape;
btSphereShape.prototype.__class__ = btSphereShape;
btSphereShape.__cache__ = {};
Module['btSphereShape'] = btSphereShape;

btSphereShape.prototype['setLocalScaling'] = btSphereShape.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btSphereShape_setLocalScaling_1(self, scaling);
};;

btSphereShape.prototype['calculateLocalInertia'] = btSphereShape.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btSphereShape_calculateLocalInertia_2(self, mass, inertia);
};;

  btSphereShape.prototype['__destroy__'] = btSphereShape.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btSphereShape___destroy___0(self);
};
// btDefaultCollisionConstructionInfo
/** @suppress {undefinedVars, duplicate} */function btDefaultCollisionConstructionInfo() {
  this.ptr = _emscripten_bind_btDefaultCollisionConstructionInfo_btDefaultCollisionConstructionInfo_0();
  getCache(btDefaultCollisionConstructionInfo)[this.ptr] = this;
};;
btDefaultCollisionConstructionInfo.prototype = Object.create(WrapperObject.prototype);
btDefaultCollisionConstructionInfo.prototype.constructor = btDefaultCollisionConstructionInfo;
btDefaultCollisionConstructionInfo.prototype.__class__ = btDefaultCollisionConstructionInfo;
btDefaultCollisionConstructionInfo.__cache__ = {};
Module['btDefaultCollisionConstructionInfo'] = btDefaultCollisionConstructionInfo;

  btDefaultCollisionConstructionInfo.prototype['__destroy__'] = btDefaultCollisionConstructionInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btDefaultCollisionConstructionInfo___destroy___0(self);
};
// btHingeConstraint
/** @suppress {undefinedVars, duplicate} */function btHingeConstraint(rbA, rbB, pivotInA, pivotInB, axisInA, axisInB, useReferenceFrameA) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (pivotInA && typeof pivotInA === 'object') pivotInA = pivotInA.ptr;
  if (pivotInB && typeof pivotInB === 'object') pivotInB = pivotInB.ptr;
  if (axisInA && typeof axisInA === 'object') axisInA = axisInA.ptr;
  if (axisInB && typeof axisInB === 'object') axisInB = axisInB.ptr;
  if (useReferenceFrameA && typeof useReferenceFrameA === 'object') useReferenceFrameA = useReferenceFrameA.ptr;
  if (pivotInA === undefined) { this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_2(rbA, rbB); getCache(btHingeConstraint)[this.ptr] = this;return }
  if (pivotInB === undefined) { this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_3(rbA, rbB, pivotInA); getCache(btHingeConstraint)[this.ptr] = this;return }
  if (axisInA === undefined) { this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_4(rbA, rbB, pivotInA, pivotInB); getCache(btHingeConstraint)[this.ptr] = this;return }
  if (axisInB === undefined) { this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_5(rbA, rbB, pivotInA, pivotInB, axisInA); getCache(btHingeConstraint)[this.ptr] = this;return }
  if (useReferenceFrameA === undefined) { this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_6(rbA, rbB, pivotInA, pivotInB, axisInA, axisInB); getCache(btHingeConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btHingeConstraint_btHingeConstraint_7(rbA, rbB, pivotInA, pivotInB, axisInA, axisInB, useReferenceFrameA);
  getCache(btHingeConstraint)[this.ptr] = this;
};;
btHingeConstraint.prototype = Object.create(btTypedConstraint.prototype);
btHingeConstraint.prototype.constructor = btHingeConstraint;
btHingeConstraint.prototype.__class__ = btHingeConstraint;
btHingeConstraint.__cache__ = {};
Module['btHingeConstraint'] = btHingeConstraint;

btHingeConstraint.prototype['setLimit'] = btHingeConstraint.prototype.setLimit = /** @suppress {undefinedVars, duplicate} */function(low, high, softness, biasFactor, relaxationFactor) {
  var self = this.ptr;
  if (low && typeof low === 'object') low = low.ptr;
  if (high && typeof high === 'object') high = high.ptr;
  if (softness && typeof softness === 'object') softness = softness.ptr;
  if (biasFactor && typeof biasFactor === 'object') biasFactor = biasFactor.ptr;
  if (relaxationFactor && typeof relaxationFactor === 'object') relaxationFactor = relaxationFactor.ptr;
  if (relaxationFactor === undefined) { _emscripten_bind_btHingeConstraint_setLimit_4(self, low, high, softness, biasFactor);  return }
  _emscripten_bind_btHingeConstraint_setLimit_5(self, low, high, softness, biasFactor, relaxationFactor);
};;

btHingeConstraint.prototype['enableAngularMotor'] = btHingeConstraint.prototype.enableAngularMotor = /** @suppress {undefinedVars, duplicate} */function(enableMotor, targetVelocity, maxMotorImpulse) {
  var self = this.ptr;
  if (enableMotor && typeof enableMotor === 'object') enableMotor = enableMotor.ptr;
  if (targetVelocity && typeof targetVelocity === 'object') targetVelocity = targetVelocity.ptr;
  if (maxMotorImpulse && typeof maxMotorImpulse === 'object') maxMotorImpulse = maxMotorImpulse.ptr;
  _emscripten_bind_btHingeConstraint_enableAngularMotor_3(self, enableMotor, targetVelocity, maxMotorImpulse);
};;

btHingeConstraint.prototype['enableFeedback'] = btHingeConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btHingeConstraint_enableFeedback_1(self, needsFeedback);
};;

btHingeConstraint.prototype['getBreakingImpulseThreshold'] = btHingeConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btHingeConstraint_getBreakingImpulseThreshold_0(self);
};;

btHingeConstraint.prototype['setBreakingImpulseThreshold'] = btHingeConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btHingeConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btHingeConstraint.prototype['__destroy__'] = btHingeConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btHingeConstraint___destroy___0(self);
};
// btConstraintSetting
/** @suppress {undefinedVars, duplicate} */function btConstraintSetting() {
  this.ptr = _emscripten_bind_btConstraintSetting_btConstraintSetting_0();
  getCache(btConstraintSetting)[this.ptr] = this;
};;
btConstraintSetting.prototype = Object.create(WrapperObject.prototype);
btConstraintSetting.prototype.constructor = btConstraintSetting;
btConstraintSetting.prototype.__class__ = btConstraintSetting;
btConstraintSetting.__cache__ = {};
Module['btConstraintSetting'] = btConstraintSetting;

  btConstraintSetting.prototype['get_m_tau'] = btConstraintSetting.prototype.get_m_tau = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btConstraintSetting_get_m_tau_0(self);
};
    btConstraintSetting.prototype['set_m_tau'] = btConstraintSetting.prototype.set_m_tau = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btConstraintSetting_set_m_tau_1(self, arg0);
};
    Object.defineProperty(btConstraintSetting.prototype, 'm_tau', { get: btConstraintSetting.prototype.get_m_tau, set: btConstraintSetting.prototype.set_m_tau });
  btConstraintSetting.prototype['get_m_damping'] = btConstraintSetting.prototype.get_m_damping = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btConstraintSetting_get_m_damping_0(self);
};
    btConstraintSetting.prototype['set_m_damping'] = btConstraintSetting.prototype.set_m_damping = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btConstraintSetting_set_m_damping_1(self, arg0);
};
    Object.defineProperty(btConstraintSetting.prototype, 'm_damping', { get: btConstraintSetting.prototype.get_m_damping, set: btConstraintSetting.prototype.set_m_damping });
  btConstraintSetting.prototype['get_m_impulseClamp'] = btConstraintSetting.prototype.get_m_impulseClamp = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btConstraintSetting_get_m_impulseClamp_0(self);
};
    btConstraintSetting.prototype['set_m_impulseClamp'] = btConstraintSetting.prototype.set_m_impulseClamp = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_btConstraintSetting_set_m_impulseClamp_1(self, arg0);
};
    Object.defineProperty(btConstraintSetting.prototype, 'm_impulseClamp', { get: btConstraintSetting.prototype.get_m_impulseClamp, set: btConstraintSetting.prototype.set_m_impulseClamp });
  btConstraintSetting.prototype['__destroy__'] = btConstraintSetting.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConstraintSetting___destroy___0(self);
};
// LocalShapeInfo
/** @suppress {undefinedVars, duplicate} */function LocalShapeInfo() { throw "cannot construct a LocalShapeInfo, no constructor in IDL" }
LocalShapeInfo.prototype = Object.create(WrapperObject.prototype);
LocalShapeInfo.prototype.constructor = LocalShapeInfo;
LocalShapeInfo.prototype.__class__ = LocalShapeInfo;
LocalShapeInfo.__cache__ = {};
Module['LocalShapeInfo'] = LocalShapeInfo;

  LocalShapeInfo.prototype['get_m_shapePart'] = LocalShapeInfo.prototype.get_m_shapePart = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_LocalShapeInfo_get_m_shapePart_0(self);
};
    LocalShapeInfo.prototype['set_m_shapePart'] = LocalShapeInfo.prototype.set_m_shapePart = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalShapeInfo_set_m_shapePart_1(self, arg0);
};
    Object.defineProperty(LocalShapeInfo.prototype, 'm_shapePart', { get: LocalShapeInfo.prototype.get_m_shapePart, set: LocalShapeInfo.prototype.set_m_shapePart });
  LocalShapeInfo.prototype['get_m_triangleIndex'] = LocalShapeInfo.prototype.get_m_triangleIndex = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_LocalShapeInfo_get_m_triangleIndex_0(self);
};
    LocalShapeInfo.prototype['set_m_triangleIndex'] = LocalShapeInfo.prototype.set_m_triangleIndex = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalShapeInfo_set_m_triangleIndex_1(self, arg0);
};
    Object.defineProperty(LocalShapeInfo.prototype, 'm_triangleIndex', { get: LocalShapeInfo.prototype.get_m_triangleIndex, set: LocalShapeInfo.prototype.set_m_triangleIndex });
  LocalShapeInfo.prototype['__destroy__'] = LocalShapeInfo.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_LocalShapeInfo___destroy___0(self);
};
// btConstraintSolver
/** @suppress {undefinedVars, duplicate} */function btConstraintSolver() { throw "cannot construct a btConstraintSolver, no constructor in IDL" }
btConstraintSolver.prototype = Object.create(WrapperObject.prototype);
btConstraintSolver.prototype.constructor = btConstraintSolver;
btConstraintSolver.prototype.__class__ = btConstraintSolver;
btConstraintSolver.__cache__ = {};
Module['btConstraintSolver'] = btConstraintSolver;

  btConstraintSolver.prototype['__destroy__'] = btConstraintSolver.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btConstraintSolver___destroy___0(self);
};
// btGeneric6DofSpringConstraint
/** @suppress {undefinedVars, duplicate} */function btGeneric6DofSpringConstraint(rbA, rbB, frameInA, frameInB, useLinearFrameReferenceFrameA) {
  if (rbA && typeof rbA === 'object') rbA = rbA.ptr;
  if (rbB && typeof rbB === 'object') rbB = rbB.ptr;
  if (frameInA && typeof frameInA === 'object') frameInA = frameInA.ptr;
  if (frameInB && typeof frameInB === 'object') frameInB = frameInB.ptr;
  if (useLinearFrameReferenceFrameA && typeof useLinearFrameReferenceFrameA === 'object') useLinearFrameReferenceFrameA = useLinearFrameReferenceFrameA.ptr;
  if (frameInB === undefined) { this.ptr = _emscripten_bind_btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_3(rbA, rbB, frameInA); getCache(btGeneric6DofSpringConstraint)[this.ptr] = this;return }
  if (useLinearFrameReferenceFrameA === undefined) { this.ptr = _emscripten_bind_btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_4(rbA, rbB, frameInA, frameInB); getCache(btGeneric6DofSpringConstraint)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_5(rbA, rbB, frameInA, frameInB, useLinearFrameReferenceFrameA);
  getCache(btGeneric6DofSpringConstraint)[this.ptr] = this;
};;
btGeneric6DofSpringConstraint.prototype = Object.create(btGeneric6DofConstraint.prototype);
btGeneric6DofSpringConstraint.prototype.constructor = btGeneric6DofSpringConstraint;
btGeneric6DofSpringConstraint.prototype.__class__ = btGeneric6DofSpringConstraint;
btGeneric6DofSpringConstraint.__cache__ = {};
Module['btGeneric6DofSpringConstraint'] = btGeneric6DofSpringConstraint;

btGeneric6DofSpringConstraint.prototype['enableSpring'] = btGeneric6DofSpringConstraint.prototype.enableSpring = /** @suppress {undefinedVars, duplicate} */function(index, onOff) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  if (onOff && typeof onOff === 'object') onOff = onOff.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_enableSpring_2(self, index, onOff);
};;

btGeneric6DofSpringConstraint.prototype['setStiffness'] = btGeneric6DofSpringConstraint.prototype.setStiffness = /** @suppress {undefinedVars, duplicate} */function(index, stiffness) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  if (stiffness && typeof stiffness === 'object') stiffness = stiffness.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setStiffness_2(self, index, stiffness);
};;

btGeneric6DofSpringConstraint.prototype['setDamping'] = btGeneric6DofSpringConstraint.prototype.setDamping = /** @suppress {undefinedVars, duplicate} */function(index, damping) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  if (damping && typeof damping === 'object') damping = damping.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setDamping_2(self, index, damping);
};;

btGeneric6DofSpringConstraint.prototype['setLinearLowerLimit'] = btGeneric6DofSpringConstraint.prototype.setLinearLowerLimit = /** @suppress {undefinedVars, duplicate} */function(linearLower) {
  var self = this.ptr;
  if (linearLower && typeof linearLower === 'object') linearLower = linearLower.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setLinearLowerLimit_1(self, linearLower);
};;

btGeneric6DofSpringConstraint.prototype['setLinearUpperLimit'] = btGeneric6DofSpringConstraint.prototype.setLinearUpperLimit = /** @suppress {undefinedVars, duplicate} */function(linearUpper) {
  var self = this.ptr;
  if (linearUpper && typeof linearUpper === 'object') linearUpper = linearUpper.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setLinearUpperLimit_1(self, linearUpper);
};;

btGeneric6DofSpringConstraint.prototype['setAngularLowerLimit'] = btGeneric6DofSpringConstraint.prototype.setAngularLowerLimit = /** @suppress {undefinedVars, duplicate} */function(angularLower) {
  var self = this.ptr;
  if (angularLower && typeof angularLower === 'object') angularLower = angularLower.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setAngularLowerLimit_1(self, angularLower);
};;

btGeneric6DofSpringConstraint.prototype['setAngularUpperLimit'] = btGeneric6DofSpringConstraint.prototype.setAngularUpperLimit = /** @suppress {undefinedVars, duplicate} */function(angularUpper) {
  var self = this.ptr;
  if (angularUpper && typeof angularUpper === 'object') angularUpper = angularUpper.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setAngularUpperLimit_1(self, angularUpper);
};;

btGeneric6DofSpringConstraint.prototype['enableFeedback'] = btGeneric6DofSpringConstraint.prototype.enableFeedback = /** @suppress {undefinedVars, duplicate} */function(needsFeedback) {
  var self = this.ptr;
  if (needsFeedback && typeof needsFeedback === 'object') needsFeedback = needsFeedback.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_enableFeedback_1(self, needsFeedback);
};;

btGeneric6DofSpringConstraint.prototype['getBreakingImpulseThreshold'] = btGeneric6DofSpringConstraint.prototype.getBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btGeneric6DofSpringConstraint_getBreakingImpulseThreshold_0(self);
};;

btGeneric6DofSpringConstraint.prototype['setBreakingImpulseThreshold'] = btGeneric6DofSpringConstraint.prototype.setBreakingImpulseThreshold = /** @suppress {undefinedVars, duplicate} */function(threshold) {
  var self = this.ptr;
  if (threshold && typeof threshold === 'object') threshold = threshold.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint_setBreakingImpulseThreshold_1(self, threshold);
};;

  btGeneric6DofSpringConstraint.prototype['__destroy__'] = btGeneric6DofSpringConstraint.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btGeneric6DofSpringConstraint___destroy___0(self);
};
// LocalConvexResult
/** @suppress {undefinedVars, duplicate} */function LocalConvexResult(hitCollisionObject, localShapeInfo, hitNormalLocal, hitPointLocal, hitFraction) {
  if (hitCollisionObject && typeof hitCollisionObject === 'object') hitCollisionObject = hitCollisionObject.ptr;
  if (localShapeInfo && typeof localShapeInfo === 'object') localShapeInfo = localShapeInfo.ptr;
  if (hitNormalLocal && typeof hitNormalLocal === 'object') hitNormalLocal = hitNormalLocal.ptr;
  if (hitPointLocal && typeof hitPointLocal === 'object') hitPointLocal = hitPointLocal.ptr;
  if (hitFraction && typeof hitFraction === 'object') hitFraction = hitFraction.ptr;
  this.ptr = _emscripten_bind_LocalConvexResult_LocalConvexResult_5(hitCollisionObject, localShapeInfo, hitNormalLocal, hitPointLocal, hitFraction);
  getCache(LocalConvexResult)[this.ptr] = this;
};;
LocalConvexResult.prototype = Object.create(WrapperObject.prototype);
LocalConvexResult.prototype.constructor = LocalConvexResult;
LocalConvexResult.prototype.__class__ = LocalConvexResult;
LocalConvexResult.__cache__ = {};
Module['LocalConvexResult'] = LocalConvexResult;

  LocalConvexResult.prototype['get_m_hitCollisionObject'] = LocalConvexResult.prototype.get_m_hitCollisionObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_LocalConvexResult_get_m_hitCollisionObject_0(self), btCollisionObject);
};
    LocalConvexResult.prototype['set_m_hitCollisionObject'] = LocalConvexResult.prototype.set_m_hitCollisionObject = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalConvexResult_set_m_hitCollisionObject_1(self, arg0);
};
    Object.defineProperty(LocalConvexResult.prototype, 'm_hitCollisionObject', { get: LocalConvexResult.prototype.get_m_hitCollisionObject, set: LocalConvexResult.prototype.set_m_hitCollisionObject });
  LocalConvexResult.prototype['get_m_localShapeInfo'] = LocalConvexResult.prototype.get_m_localShapeInfo = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_LocalConvexResult_get_m_localShapeInfo_0(self), LocalShapeInfo);
};
    LocalConvexResult.prototype['set_m_localShapeInfo'] = LocalConvexResult.prototype.set_m_localShapeInfo = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalConvexResult_set_m_localShapeInfo_1(self, arg0);
};
    Object.defineProperty(LocalConvexResult.prototype, 'm_localShapeInfo', { get: LocalConvexResult.prototype.get_m_localShapeInfo, set: LocalConvexResult.prototype.set_m_localShapeInfo });
  LocalConvexResult.prototype['get_m_hitNormalLocal'] = LocalConvexResult.prototype.get_m_hitNormalLocal = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_LocalConvexResult_get_m_hitNormalLocal_0(self), btVector3);
};
    LocalConvexResult.prototype['set_m_hitNormalLocal'] = LocalConvexResult.prototype.set_m_hitNormalLocal = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalConvexResult_set_m_hitNormalLocal_1(self, arg0);
};
    Object.defineProperty(LocalConvexResult.prototype, 'm_hitNormalLocal', { get: LocalConvexResult.prototype.get_m_hitNormalLocal, set: LocalConvexResult.prototype.set_m_hitNormalLocal });
  LocalConvexResult.prototype['get_m_hitPointLocal'] = LocalConvexResult.prototype.get_m_hitPointLocal = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_LocalConvexResult_get_m_hitPointLocal_0(self), btVector3);
};
    LocalConvexResult.prototype['set_m_hitPointLocal'] = LocalConvexResult.prototype.set_m_hitPointLocal = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalConvexResult_set_m_hitPointLocal_1(self, arg0);
};
    Object.defineProperty(LocalConvexResult.prototype, 'm_hitPointLocal', { get: LocalConvexResult.prototype.get_m_hitPointLocal, set: LocalConvexResult.prototype.set_m_hitPointLocal });
  LocalConvexResult.prototype['get_m_hitFraction'] = LocalConvexResult.prototype.get_m_hitFraction = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_LocalConvexResult_get_m_hitFraction_0(self);
};
    LocalConvexResult.prototype['set_m_hitFraction'] = LocalConvexResult.prototype.set_m_hitFraction = /** @suppress {undefinedVars, duplicate} */function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LocalConvexResult_set_m_hitFraction_1(self, arg0);
};
    Object.defineProperty(LocalConvexResult.prototype, 'm_hitFraction', { get: LocalConvexResult.prototype.get_m_hitFraction, set: LocalConvexResult.prototype.set_m_hitFraction });
  LocalConvexResult.prototype['__destroy__'] = LocalConvexResult.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_LocalConvexResult___destroy___0(self);
};
// btOverlappingPairCallback
/** @suppress {undefinedVars, duplicate} */function btOverlappingPairCallback() { throw "cannot construct a btOverlappingPairCallback, no constructor in IDL" }
btOverlappingPairCallback.prototype = Object.create(WrapperObject.prototype);
btOverlappingPairCallback.prototype.constructor = btOverlappingPairCallback;
btOverlappingPairCallback.prototype.__class__ = btOverlappingPairCallback;
btOverlappingPairCallback.__cache__ = {};
Module['btOverlappingPairCallback'] = btOverlappingPairCallback;

  btOverlappingPairCallback.prototype['__destroy__'] = btOverlappingPairCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btOverlappingPairCallback___destroy___0(self);
};
// btRigidBody
/** @suppress {undefinedVars, duplicate} */function btRigidBody(constructionInfo) {
  if (constructionInfo && typeof constructionInfo === 'object') constructionInfo = constructionInfo.ptr;
  this.ptr = _emscripten_bind_btRigidBody_btRigidBody_1(constructionInfo);
  getCache(btRigidBody)[this.ptr] = this;
};;
btRigidBody.prototype = Object.create(btCollisionObject.prototype);
btRigidBody.prototype.constructor = btRigidBody;
btRigidBody.prototype.__class__ = btRigidBody;
btRigidBody.__cache__ = {};
Module['btRigidBody'] = btRigidBody;

btRigidBody.prototype['getCenterOfMassTransform'] = btRigidBody.prototype.getCenterOfMassTransform = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getCenterOfMassTransform_0(self), btTransform);
};;

btRigidBody.prototype['setCenterOfMassTransform'] = btRigidBody.prototype.setCenterOfMassTransform = /** @suppress {undefinedVars, duplicate} */function(xform) {
  var self = this.ptr;
  if (xform && typeof xform === 'object') xform = xform.ptr;
  _emscripten_bind_btRigidBody_setCenterOfMassTransform_1(self, xform);
};;

btRigidBody.prototype['setSleepingThresholds'] = btRigidBody.prototype.setSleepingThresholds = /** @suppress {undefinedVars, duplicate} */function(linear, angular) {
  var self = this.ptr;
  if (linear && typeof linear === 'object') linear = linear.ptr;
  if (angular && typeof angular === 'object') angular = angular.ptr;
  _emscripten_bind_btRigidBody_setSleepingThresholds_2(self, linear, angular);
};;

btRigidBody.prototype['setDamping'] = btRigidBody.prototype.setDamping = /** @suppress {undefinedVars, duplicate} */function(lin_damping, ang_damping) {
  var self = this.ptr;
  if (lin_damping && typeof lin_damping === 'object') lin_damping = lin_damping.ptr;
  if (ang_damping && typeof ang_damping === 'object') ang_damping = ang_damping.ptr;
  _emscripten_bind_btRigidBody_setDamping_2(self, lin_damping, ang_damping);
};;

btRigidBody.prototype['setMassProps'] = btRigidBody.prototype.setMassProps = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btRigidBody_setMassProps_2(self, mass, inertia);
};;

btRigidBody.prototype['setLinearFactor'] = btRigidBody.prototype.setLinearFactor = /** @suppress {undefinedVars, duplicate} */function(linearFactor) {
  var self = this.ptr;
  if (linearFactor && typeof linearFactor === 'object') linearFactor = linearFactor.ptr;
  _emscripten_bind_btRigidBody_setLinearFactor_1(self, linearFactor);
};;

btRigidBody.prototype['applyTorque'] = btRigidBody.prototype.applyTorque = /** @suppress {undefinedVars, duplicate} */function(torque) {
  var self = this.ptr;
  if (torque && typeof torque === 'object') torque = torque.ptr;
  _emscripten_bind_btRigidBody_applyTorque_1(self, torque);
};;

btRigidBody.prototype['applyForce'] = btRigidBody.prototype.applyForce = /** @suppress {undefinedVars, duplicate} */function(force, rel_pos) {
  var self = this.ptr;
  if (force && typeof force === 'object') force = force.ptr;
  if (rel_pos && typeof rel_pos === 'object') rel_pos = rel_pos.ptr;
  _emscripten_bind_btRigidBody_applyForce_2(self, force, rel_pos);
};;

btRigidBody.prototype['applyCentralForce'] = btRigidBody.prototype.applyCentralForce = /** @suppress {undefinedVars, duplicate} */function(force) {
  var self = this.ptr;
  if (force && typeof force === 'object') force = force.ptr;
  _emscripten_bind_btRigidBody_applyCentralForce_1(self, force);
};;

btRigidBody.prototype['applyTorqueImpulse'] = btRigidBody.prototype.applyTorqueImpulse = /** @suppress {undefinedVars, duplicate} */function(torque) {
  var self = this.ptr;
  if (torque && typeof torque === 'object') torque = torque.ptr;
  _emscripten_bind_btRigidBody_applyTorqueImpulse_1(self, torque);
};;

btRigidBody.prototype['applyImpulse'] = btRigidBody.prototype.applyImpulse = /** @suppress {undefinedVars, duplicate} */function(impulse, rel_pos) {
  var self = this.ptr;
  if (impulse && typeof impulse === 'object') impulse = impulse.ptr;
  if (rel_pos && typeof rel_pos === 'object') rel_pos = rel_pos.ptr;
  _emscripten_bind_btRigidBody_applyImpulse_2(self, impulse, rel_pos);
};;

btRigidBody.prototype['applyCentralImpulse'] = btRigidBody.prototype.applyCentralImpulse = /** @suppress {undefinedVars, duplicate} */function(impulse) {
  var self = this.ptr;
  if (impulse && typeof impulse === 'object') impulse = impulse.ptr;
  _emscripten_bind_btRigidBody_applyCentralImpulse_1(self, impulse);
};;

btRigidBody.prototype['updateInertiaTensor'] = btRigidBody.prototype.updateInertiaTensor = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btRigidBody_updateInertiaTensor_0(self);
};;

btRigidBody.prototype['getLinearVelocity'] = btRigidBody.prototype.getLinearVelocity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getLinearVelocity_0(self), btVector3);
};;

btRigidBody.prototype['getAngularVelocity'] = btRigidBody.prototype.getAngularVelocity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getAngularVelocity_0(self), btVector3);
};;

btRigidBody.prototype['setLinearVelocity'] = btRigidBody.prototype.setLinearVelocity = /** @suppress {undefinedVars, duplicate} */function(lin_vel) {
  var self = this.ptr;
  if (lin_vel && typeof lin_vel === 'object') lin_vel = lin_vel.ptr;
  _emscripten_bind_btRigidBody_setLinearVelocity_1(self, lin_vel);
};;

btRigidBody.prototype['setAngularVelocity'] = btRigidBody.prototype.setAngularVelocity = /** @suppress {undefinedVars, duplicate} */function(ang_vel) {
  var self = this.ptr;
  if (ang_vel && typeof ang_vel === 'object') ang_vel = ang_vel.ptr;
  _emscripten_bind_btRigidBody_setAngularVelocity_1(self, ang_vel);
};;

btRigidBody.prototype['getMotionState'] = btRigidBody.prototype.getMotionState = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getMotionState_0(self), btMotionState);
};;

btRigidBody.prototype['setAngularFactor'] = btRigidBody.prototype.setAngularFactor = /** @suppress {undefinedVars, duplicate} */function(angularFactor) {
  var self = this.ptr;
  if (angularFactor && typeof angularFactor === 'object') angularFactor = angularFactor.ptr;
  _emscripten_bind_btRigidBody_setAngularFactor_1(self, angularFactor);
};;

btRigidBody.prototype['upcast'] = btRigidBody.prototype.upcast = /** @suppress {undefinedVars, duplicate} */function(colObj) {
  var self = this.ptr;
  if (colObj && typeof colObj === 'object') colObj = colObj.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_upcast_1(self, colObj), btRigidBody);
};;

btRigidBody.prototype['setAnisotropicFriction'] = btRigidBody.prototype.setAnisotropicFriction = /** @suppress {undefinedVars, duplicate} */function(anisotropicFriction, frictionMode) {
  var self = this.ptr;
  if (anisotropicFriction && typeof anisotropicFriction === 'object') anisotropicFriction = anisotropicFriction.ptr;
  if (frictionMode && typeof frictionMode === 'object') frictionMode = frictionMode.ptr;
  _emscripten_bind_btRigidBody_setAnisotropicFriction_2(self, anisotropicFriction, frictionMode);
};;

btRigidBody.prototype['getCollisionShape'] = btRigidBody.prototype.getCollisionShape = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getCollisionShape_0(self), btCollisionShape);
};;

btRigidBody.prototype['setContactProcessingThreshold'] = btRigidBody.prototype.setContactProcessingThreshold = /** @suppress {undefinedVars, duplicate} */function(contactProcessingThreshold) {
  var self = this.ptr;
  if (contactProcessingThreshold && typeof contactProcessingThreshold === 'object') contactProcessingThreshold = contactProcessingThreshold.ptr;
  _emscripten_bind_btRigidBody_setContactProcessingThreshold_1(self, contactProcessingThreshold);
};;

btRigidBody.prototype['setActivationState'] = btRigidBody.prototype.setActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btRigidBody_setActivationState_1(self, newState);
};;

btRigidBody.prototype['forceActivationState'] = btRigidBody.prototype.forceActivationState = /** @suppress {undefinedVars, duplicate} */function(newState) {
  var self = this.ptr;
  if (newState && typeof newState === 'object') newState = newState.ptr;
  _emscripten_bind_btRigidBody_forceActivationState_1(self, newState);
};;

btRigidBody.prototype['activate'] = btRigidBody.prototype.activate = /** @suppress {undefinedVars, duplicate} */function(forceActivation) {
  var self = this.ptr;
  if (forceActivation && typeof forceActivation === 'object') forceActivation = forceActivation.ptr;
  if (forceActivation === undefined) { _emscripten_bind_btRigidBody_activate_0(self);  return }
  _emscripten_bind_btRigidBody_activate_1(self, forceActivation);
};;

btRigidBody.prototype['isActive'] = btRigidBody.prototype.isActive = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btRigidBody_isActive_0(self));
};;

btRigidBody.prototype['isKinematicObject'] = btRigidBody.prototype.isKinematicObject = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return !!(_emscripten_bind_btRigidBody_isKinematicObject_0(self));
};;

btRigidBody.prototype['setRestitution'] = btRigidBody.prototype.setRestitution = /** @suppress {undefinedVars, duplicate} */function(rest) {
  var self = this.ptr;
  if (rest && typeof rest === 'object') rest = rest.ptr;
  _emscripten_bind_btRigidBody_setRestitution_1(self, rest);
};;

btRigidBody.prototype['setFriction'] = btRigidBody.prototype.setFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btRigidBody_setFriction_1(self, frict);
};;

btRigidBody.prototype['setRollingFriction'] = btRigidBody.prototype.setRollingFriction = /** @suppress {undefinedVars, duplicate} */function(frict) {
  var self = this.ptr;
  if (frict && typeof frict === 'object') frict = frict.ptr;
  _emscripten_bind_btRigidBody_setRollingFriction_1(self, frict);
};;

btRigidBody.prototype['getWorldTransform'] = btRigidBody.prototype.getWorldTransform = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRigidBody_getWorldTransform_0(self), btTransform);
};;

btRigidBody.prototype['getCollisionFlags'] = btRigidBody.prototype.getCollisionFlags = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRigidBody_getCollisionFlags_0(self);
};;

btRigidBody.prototype['setCollisionFlags'] = btRigidBody.prototype.setCollisionFlags = /** @suppress {undefinedVars, duplicate} */function(flags) {
  var self = this.ptr;
  if (flags && typeof flags === 'object') flags = flags.ptr;
  _emscripten_bind_btRigidBody_setCollisionFlags_1(self, flags);
};;

btRigidBody.prototype['setWorldTransform'] = btRigidBody.prototype.setWorldTransform = /** @suppress {undefinedVars, duplicate} */function(worldTrans) {
  var self = this.ptr;
  if (worldTrans && typeof worldTrans === 'object') worldTrans = worldTrans.ptr;
  _emscripten_bind_btRigidBody_setWorldTransform_1(self, worldTrans);
};;

btRigidBody.prototype['setCollisionShape'] = btRigidBody.prototype.setCollisionShape = /** @suppress {undefinedVars, duplicate} */function(collisionShape) {
  var self = this.ptr;
  if (collisionShape && typeof collisionShape === 'object') collisionShape = collisionShape.ptr;
  _emscripten_bind_btRigidBody_setCollisionShape_1(self, collisionShape);
};;

btRigidBody.prototype['setCcdMotionThreshold'] = btRigidBody.prototype.setCcdMotionThreshold = /** @suppress {undefinedVars, duplicate} */function(ccdMotionThreshold) {
  var self = this.ptr;
  if (ccdMotionThreshold && typeof ccdMotionThreshold === 'object') ccdMotionThreshold = ccdMotionThreshold.ptr;
  _emscripten_bind_btRigidBody_setCcdMotionThreshold_1(self, ccdMotionThreshold);
};;

btRigidBody.prototype['setCcdSweptSphereRadius'] = btRigidBody.prototype.setCcdSweptSphereRadius = /** @suppress {undefinedVars, duplicate} */function(radius) {
  var self = this.ptr;
  if (radius && typeof radius === 'object') radius = radius.ptr;
  _emscripten_bind_btRigidBody_setCcdSweptSphereRadius_1(self, radius);
};;

  btRigidBody.prototype['__destroy__'] = btRigidBody.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btRigidBody___destroy___0(self);
};
// btCylinderShapeX
/** @suppress {undefinedVars, duplicate} */function btCylinderShapeX(halfExtents) {
  if (halfExtents && typeof halfExtents === 'object') halfExtents = halfExtents.ptr;
  this.ptr = _emscripten_bind_btCylinderShapeX_btCylinderShapeX_1(halfExtents);
  getCache(btCylinderShapeX)[this.ptr] = this;
};;
btCylinderShapeX.prototype = Object.create(btCylinderShape.prototype);
btCylinderShapeX.prototype.constructor = btCylinderShapeX;
btCylinderShapeX.prototype.__class__ = btCylinderShapeX;
btCylinderShapeX.__cache__ = {};
Module['btCylinderShapeX'] = btCylinderShapeX;

btCylinderShapeX.prototype['setLocalScaling'] = btCylinderShapeX.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCylinderShapeX_setLocalScaling_1(self, scaling);
};;

btCylinderShapeX.prototype['calculateLocalInertia'] = btCylinderShapeX.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCylinderShapeX_calculateLocalInertia_2(self, mass, inertia);
};;

  btCylinderShapeX.prototype['__destroy__'] = btCylinderShapeX.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCylinderShapeX___destroy___0(self);
};
// btCylinderShapeZ
/** @suppress {undefinedVars, duplicate} */function btCylinderShapeZ(halfExtents) {
  if (halfExtents && typeof halfExtents === 'object') halfExtents = halfExtents.ptr;
  this.ptr = _emscripten_bind_btCylinderShapeZ_btCylinderShapeZ_1(halfExtents);
  getCache(btCylinderShapeZ)[this.ptr] = this;
};;
btCylinderShapeZ.prototype = Object.create(btCylinderShape.prototype);
btCylinderShapeZ.prototype.constructor = btCylinderShapeZ;
btCylinderShapeZ.prototype.__class__ = btCylinderShapeZ;
btCylinderShapeZ.__cache__ = {};
Module['btCylinderShapeZ'] = btCylinderShapeZ;

btCylinderShapeZ.prototype['setLocalScaling'] = btCylinderShapeZ.prototype.setLocalScaling = /** @suppress {undefinedVars, duplicate} */function(scaling) {
  var self = this.ptr;
  if (scaling && typeof scaling === 'object') scaling = scaling.ptr;
  _emscripten_bind_btCylinderShapeZ_setLocalScaling_1(self, scaling);
};;

btCylinderShapeZ.prototype['calculateLocalInertia'] = btCylinderShapeZ.prototype.calculateLocalInertia = /** @suppress {undefinedVars, duplicate} */function(mass, inertia) {
  var self = this.ptr;
  if (mass && typeof mass === 'object') mass = mass.ptr;
  if (inertia && typeof inertia === 'object') inertia = inertia.ptr;
  _emscripten_bind_btCylinderShapeZ_calculateLocalInertia_2(self, mass, inertia);
};;

  btCylinderShapeZ.prototype['__destroy__'] = btCylinderShapeZ.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btCylinderShapeZ___destroy___0(self);
};
// btTransform
/** @suppress {undefinedVars, duplicate} */function btTransform(q, v) {
  if (q && typeof q === 'object') q = q.ptr;
  if (v && typeof v === 'object') v = v.ptr;
  if (q === undefined) { this.ptr = _emscripten_bind_btTransform_btTransform_0(); getCache(btTransform)[this.ptr] = this;return }
  if (v === undefined) { this.ptr = _emscripten_bind_btTransform_btTransform_1(q); getCache(btTransform)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_btTransform_btTransform_2(q, v);
  getCache(btTransform)[this.ptr] = this;
};;
btTransform.prototype = Object.create(WrapperObject.prototype);
btTransform.prototype.constructor = btTransform;
btTransform.prototype.__class__ = btTransform;
btTransform.__cache__ = {};
Module['btTransform'] = btTransform;

btTransform.prototype['setIdentity'] = btTransform.prototype.setIdentity = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btTransform_setIdentity_0(self);
};;

btTransform.prototype['setOrigin'] = btTransform.prototype.setOrigin = /** @suppress {undefinedVars, duplicate} */function(origin) {
  var self = this.ptr;
  if (origin && typeof origin === 'object') origin = origin.ptr;
  _emscripten_bind_btTransform_setOrigin_1(self, origin);
};;

btTransform.prototype['setRotation'] = btTransform.prototype.setRotation = /** @suppress {undefinedVars, duplicate} */function(rotation) {
  var self = this.ptr;
  if (rotation && typeof rotation === 'object') rotation = rotation.ptr;
  _emscripten_bind_btTransform_setRotation_1(self, rotation);
};;

btTransform.prototype['getOrigin'] = btTransform.prototype.getOrigin = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btTransform_getOrigin_0(self), btVector3);
};;

btTransform.prototype['getRotation'] = btTransform.prototype.getRotation = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btTransform_getRotation_0(self), btQuaternion);
};;

btTransform.prototype['getBasis'] = btTransform.prototype.getBasis = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btTransform_getBasis_0(self), btMatrix3x3);
};;

  btTransform.prototype['__destroy__'] = btTransform.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btTransform___destroy___0(self);
};
// btRaycastVehicle
/** @suppress {undefinedVars, duplicate} */function btRaycastVehicle(tuning, chassis, raycaster) {
  if (tuning && typeof tuning === 'object') tuning = tuning.ptr;
  if (chassis && typeof chassis === 'object') chassis = chassis.ptr;
  if (raycaster && typeof raycaster === 'object') raycaster = raycaster.ptr;
  this.ptr = _emscripten_bind_btRaycastVehicle_btRaycastVehicle_3(tuning, chassis, raycaster);
  getCache(btRaycastVehicle)[this.ptr] = this;
};;
btRaycastVehicle.prototype = Object.create(btActionInterface.prototype);
btRaycastVehicle.prototype.constructor = btRaycastVehicle;
btRaycastVehicle.prototype.__class__ = btRaycastVehicle;
btRaycastVehicle.__cache__ = {};
Module['btRaycastVehicle'] = btRaycastVehicle;

btRaycastVehicle.prototype['applyEngineForce'] = btRaycastVehicle.prototype.applyEngineForce = /** @suppress {undefinedVars, duplicate} */function(force, wheel) {
  var self = this.ptr;
  if (force && typeof force === 'object') force = force.ptr;
  if (wheel && typeof wheel === 'object') wheel = wheel.ptr;
  _emscripten_bind_btRaycastVehicle_applyEngineForce_2(self, force, wheel);
};;

btRaycastVehicle.prototype['setSteeringValue'] = btRaycastVehicle.prototype.setSteeringValue = /** @suppress {undefinedVars, duplicate} */function(steering, wheel) {
  var self = this.ptr;
  if (steering && typeof steering === 'object') steering = steering.ptr;
  if (wheel && typeof wheel === 'object') wheel = wheel.ptr;
  _emscripten_bind_btRaycastVehicle_setSteeringValue_2(self, steering, wheel);
};;

btRaycastVehicle.prototype['getWheelTransformWS'] = btRaycastVehicle.prototype.getWheelTransformWS = /** @suppress {undefinedVars, duplicate} */function(wheelIndex) {
  var self = this.ptr;
  if (wheelIndex && typeof wheelIndex === 'object') wheelIndex = wheelIndex.ptr;
  return wrapPointer(_emscripten_bind_btRaycastVehicle_getWheelTransformWS_1(self, wheelIndex), btTransform);
};;

btRaycastVehicle.prototype['updateWheelTransform'] = btRaycastVehicle.prototype.updateWheelTransform = /** @suppress {undefinedVars, duplicate} */function(wheelIndex, interpolatedTransform) {
  var self = this.ptr;
  if (wheelIndex && typeof wheelIndex === 'object') wheelIndex = wheelIndex.ptr;
  if (interpolatedTransform && typeof interpolatedTransform === 'object') interpolatedTransform = interpolatedTransform.ptr;
  _emscripten_bind_btRaycastVehicle_updateWheelTransform_2(self, wheelIndex, interpolatedTransform);
};;

btRaycastVehicle.prototype['addWheel'] = btRaycastVehicle.prototype.addWheel = /** @suppress {undefinedVars, duplicate} */function(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel) {
  var self = this.ptr;
  if (connectionPointCS0 && typeof connectionPointCS0 === 'object') connectionPointCS0 = connectionPointCS0.ptr;
  if (wheelDirectionCS0 && typeof wheelDirectionCS0 === 'object') wheelDirectionCS0 = wheelDirectionCS0.ptr;
  if (wheelAxleCS && typeof wheelAxleCS === 'object') wheelAxleCS = wheelAxleCS.ptr;
  if (suspensionRestLength && typeof suspensionRestLength === 'object') suspensionRestLength = suspensionRestLength.ptr;
  if (wheelRadius && typeof wheelRadius === 'object') wheelRadius = wheelRadius.ptr;
  if (tuning && typeof tuning === 'object') tuning = tuning.ptr;
  if (isFrontWheel && typeof isFrontWheel === 'object') isFrontWheel = isFrontWheel.ptr;
  return wrapPointer(_emscripten_bind_btRaycastVehicle_addWheel_7(self, connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel), btWheelInfo);
};;

btRaycastVehicle.prototype['getNumWheels'] = btRaycastVehicle.prototype.getNumWheels = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return _emscripten_bind_btRaycastVehicle_getNumWheels_0(self);
};;

btRaycastVehicle.prototype['getRigidBody'] = btRaycastVehicle.prototype.getRigidBody = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_btRaycastVehicle_getRigidBody_0(self), btRigidBody);
};;

btRaycastVehicle.prototype['getWheelInfo'] = btRaycastVehicle.prototype.getWheelInfo = /** @suppress {undefinedVars, duplicate} */function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return wrapPointer(_emscripten_bind_btRaycastVehicle_getWheelInfo_1(self, index), btWheelInfo);
};;

btRaycastVehicle.prototype['setBrake'] = btRaycastVehicle.prototype.setBrake = /** @suppress {undefinedVars, duplicate} */function(brake, wheelIndex) {
  var self = this.ptr;
  if (brake && typeof brake === 'object') brake = brake.ptr;
  if (wheelIndex && typeof wheelIndex === 'object') wheelIndex = wheelIndex.ptr;
  _emscripten_bind_btRaycastVehicle_setBrake_2(self, brake, wheelIndex);
};;

btRaycastVehicle.prototype['setCoordinateSystem'] = btRaycastVehicle.prototype.setCoordinateSystem = /** @suppress {undefinedVars, duplicate} */function(rightIndex, upIndex, forwardIndex) {
  var self = this.ptr;
  if (rightIndex && typeof rightIndex === 'object') rightIndex = rightIndex.ptr;
  if (upIndex && typeof upIndex === 'object') upIndex = upIndex.ptr;
  if (forwardIndex && typeof forwardIndex === 'object') forwardIndex = forwardIndex.ptr;
  _emscripten_bind_btRaycastVehicle_setCoordinateSystem_3(self, rightIndex, upIndex, forwardIndex);
};;

  btRaycastVehicle.prototype['__destroy__'] = btRaycastVehicle.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btRaycastVehicle___destroy___0(self);
};
// btGhostPairCallback
/** @suppress {undefinedVars, duplicate} */function btGhostPairCallback() {
  this.ptr = _emscripten_bind_btGhostPairCallback_btGhostPairCallback_0();
  getCache(btGhostPairCallback)[this.ptr] = this;
};;
btGhostPairCallback.prototype = Object.create(WrapperObject.prototype);
btGhostPairCallback.prototype.constructor = btGhostPairCallback;
btGhostPairCallback.prototype.__class__ = btGhostPairCallback;
btGhostPairCallback.__cache__ = {};
Module['btGhostPairCallback'] = btGhostPairCallback;

  btGhostPairCallback.prototype['__destroy__'] = btGhostPairCallback.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} */function() {
  var self = this.ptr;
  _emscripten_bind_btGhostPairCallback___destroy___0(self);
};
(function() {
  function setupEnums() {
    
  }
  if (runtimeInitialized) setupEnums();
  else addOnPreMain(setupEnums);
})();
