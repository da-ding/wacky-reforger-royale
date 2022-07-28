class radiusScale {

		override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		World w = GetGame().GetWorld();
		size -= 0.05;
		
		vector transform[4];
		
		auto usTrigger = SCR_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
		float lastRadius = usTrigger.GetSphereRadius();
		float radius = lastRadius - (timeSlice * 3);
		usTrigger.SetSphereRadius(radius);
		auto ussrTrigger = SCR_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUSSR"));
		ussrTrigger.SetSphereRadius(radius);
		
		float scale = radius / lastRadius;

		IEntity barrier = IEntity.Cast(GetGame().GetWorld().FindEntityByName("Barrier"));
		if (!barrier) return;

		vector transform[4];
		barrier.GetWorldTransform(transform);
		if (!transform) return;
		transform[3] = transform[3] + "0 0.01 0"; // slightly raise because origin is below ground
		SCR_Math3D.ScaleMatrix(transform, scale);

		barrier.SetWorldTransform(transform);
	}
}