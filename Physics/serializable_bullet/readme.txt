If someone decide to use bullet physics in project
hope this would be helpfull.

Serializeble bullet animator class 0.2

CBulletAnimatorManager
	implements ISceneNodeAnimatorFactory interface,
	creates animators and manage bullet objects,
	can control several bullet worlds
	
CBulletWorldAnimator
	pseudo-animator, should be assigned to parent scene
	to be able to deserialize self
	
CBulletObjectAnimator
	animates scene node, currently supports sphere,
	box, convex, static concave and GIMPACT concave shapes
	
	
You should have bullet library 2.53.

TODO:
1. Cached bullet meshes
2. Compound shapes and etc.