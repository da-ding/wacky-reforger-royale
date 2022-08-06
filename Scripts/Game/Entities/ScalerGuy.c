class ScalerGuyClass: DAD_BaseTriggerEntityClass
{
};

class ScalerGuy: DAD_BaseTriggerEntity
{
	AIWaypoint m_eWaypoint;

	float m_fInitialTriggerRadius;
	float m_fInitialBarrierScale;

	[RplProp(onRplName: "UpdateBarrierEntity")]
	float m_fBarrierScale;
	//[RplProp(onRplName: "UpdateBarrierEntity")]
	vector m_vBarrierOrigin;

	void UpdateBarrierEntity()
	{
		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");
		if (!barrier) return;

		if (m_fBarrierScale) barrier.SetScale(m_fBarrierScale);
		if (m_vBarrierOrigin) barrier.SetOrigin(m_vBarrierOrigin);
		barrier.Update();
	}

	void ScaleBarrier(float timeSlice)
	{
		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		if (!rplC || !rplC.IsOwner()) return;

		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");

		DAD_FactionControlTriggerEntity trigger = DAD_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));

		float delta = (timeSlice * 0.8);
		float radius = trigger.GetSphereRadius() - delta;

		trigger.SetSphereRadius(radius);
		if (m_eWaypoint)
			m_eWaypoint.SetCompletionRadius(radius);

		
		//--- Calculate shrink transformation of Barrier

		float scale = radius / m_fInitialTriggerRadius;
		// Set the barrier size to a fraction of its initial size
		m_fBarrierScale = scale * m_fInitialBarrierScale;
		// slightly raise because greenhouse origin is below ground
		m_vBarrierOrigin = barrier.GetOrigin() + ("0 0.15 0" * delta);

		UpdateBarrierEntity();
		Replication.BumpMe();
	}

	void init()
	{

		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));

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

		const float tick = 1 / 32;
		//const float tick = 2;

		GetGame().GetCallqueue().CallLater(ScaleBarrier, tick * 1000, true, tick);
	}

	override void OnActivate(IEntity ent)
	{
		super.OnActivate(ent);
	}

	override void OnInit(IEntity owner)
	{
		super.OnInit(owner);
		GetGame().GetCallqueue().CallLater(init, 200);
	}

};
