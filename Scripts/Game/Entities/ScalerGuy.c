class ScalerGuyClass: DAD_BaseTriggerEntityClass
{
};

class ScalerGuy: DAD_BaseTriggerEntity
{
	// user script
	AIWaypoint m_eWaypoint;

	float m_fInitialTriggerRadius;
	float m_fInitialBarrierScale;

	[RplProp(onRplName: "UpdateBarrierEntity")]
	float m_fBarrierScale;
	//[RplProp(onRplName: "UpdateBarrierEntity")]
	vector m_vBarrierOrigin;

	void UpdateBarrierEntity()
	{
		Print("Oh Hey!! UpdateBarrier is running!");
		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");
		if (!barrier)
		{
			Print("No Barrier found for Update!");
			return;
		}
		if (m_fBarrierScale) barrier.SetScale(m_fBarrierScale);
		if (m_vBarrierOrigin) barrier.SetOrigin(m_vBarrierOrigin);
		barrier.Update();
	}

	void ScaleBarrier(float timeSlice)
	{
		Print("Scale Barrier");
		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		//if (!Replication.IsServer()) return;
		//if (!rplC || !rplC.IsOwner()) return;

		if (Replication.IsClient()) return;
		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");

		if (true) {
			DAD_FactionControlTriggerEntity trigger = DAD_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));

			float delta = (timeSlice * 3);
			float radius = trigger.GetSphereRadius() - delta;

			trigger.SetSphereRadius(radius);


			float scale = radius / m_fInitialTriggerRadius;
			// Set the barrier size to a fraction of its initial size
			m_fBarrierScale = scale * m_fInitialBarrierScale;
			// slightly raise because greenhouse origin is below ground
			m_vBarrierOrigin = barrier.GetOrigin() + ("0 0.15 0" * delta);
			UpdateBarrierEntity();
			Replication.BumpMe();

			if (m_eWaypoint)
				m_eWaypoint.SetCompletionRadius(radius);
		}
	}

	void init()
	{

		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		Print("Game Start");

		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");
		if (barrier) Print("Barrier exists in game start");

		if (Replication.IsServer())
		{

			DAD_FactionControlTriggerEntity trigger = DAD_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
			m_fInitialTriggerRadius = trigger.GetSphereRadius();

			m_fInitialBarrierScale = barrier.GetScale();

			Resource scoutRes = Resource.Load("{A88F0B6CF25BD1DE}Prefabs/AI/Waypoints/AIWaypoint_Scout.et");
			m_eWaypoint = AIWaypoint.Cast(SpawnHelpers.SpawnEntity(scoutRes, trigger.GetOrigin()));
			m_eWaypoint.SetName("BarrierWaypoint");

			Resource rplTest = Resource.Load("{E407D9BCB4B30DC6}Prefabs/MP/Test_RplEntity.et");
		}
		else
		{
		}
		const float tick = 1 / 16;
		//const float tick = 2;
		Print("End Game Start 1 ");
		GetGame().GetCallqueue().CallLater(ScaleBarrier, tick * 1000, true, tick);
		//GetGame().GetCallqueue().CallLater(barrier.Update, 500, true);
		Print("End Game Start");
	}

	override void OnActivate(IEntity ent)
	{
		/* code here */
	}

	override void OnInit(IEntity owner)
	{
		/* code here */
		super.OnInit(owner);
		GetGame().GetCallqueue().CallLater(init, 200);
	}

};
