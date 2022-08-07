class ScalerGuyClass: DAD_BaseTriggerEntityClass
{
};

class ScalerGuy: DAD_BaseTriggerEntity
{
	AIWaypoint m_eWaypoint;

	float m_fInitialTriggerRadius;
	float m_fInitialBarrierScale;

	[RplProp()]
	float m_fBarrierOriginX;
	[RplProp()]
	float m_fBarrierOriginY;
	[RplProp()]
	float m_fBarrierOriginZ;
	[RplProp(onRplName: "UpdateBarrierEntity")]
	float m_fBarrierScale;

	void UpdateBarrierEntity()
	{
		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");
		if (!barrier) return;

		if (m_fBarrierScale) barrier.SetScale(m_fBarrierScale);
		if (m_fBarrierOriginX && m_fBarrierOriginY && m_fBarrierOriginZ)
			barrier.SetOrigin(Vector(m_fBarrierOriginX, m_fBarrierOriginY, m_fBarrierOriginZ));
		barrier.Update();
	}

	void ScaleBarrier(float timeSlice)
	{
		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		if (!rplC || !rplC.IsOwner()) return;

		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");

		SCR_FactionControlTriggerEntity trigger = SCR_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
		SCR_FactionControlTriggerEntity triggerRus = SCR_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUSSR"));

		float oldRadius = trigger.GetSphereRadius();
		float metresDecreasePerSecond = 1;
		if (oldRadius < 50)
			metresDecreasePerSecond = Math.Min(metresDecreasePerSecond, oldRadius * 0.03);

		float delta = (timeSlice * metresDecreasePerSecond);
		float radius = oldRadius - delta;

		trigger.SetSphereRadius(radius);
		triggerRus.SetSphereRadius(radius);
		if (m_eWaypoint)
			m_eWaypoint.SetCompletionRadius(radius);

		
		//--- Calculate shrink transformation of Barrier

		float scale = radius / m_fInitialTriggerRadius;
		// Set the barrier size to a fraction of its initial size
		m_fBarrierScale = scale * m_fInitialBarrierScale;
		// slightly raise because greenhouse origin is below ground
		// TODO: Calculate the lowest point of ground along the barrier base
		//  and make sure the greenhouse dirt is below that
		float nearEndOfGameHack;
		if (scale < 0.15) nearEndOfGameHack = 1.4;
		else              nearEndOfGameHack = 1;
		vector origin = barrier.GetOrigin() + ("0 0.15 0" * delta * nearEndOfGameHack);
		m_fBarrierOriginX = vector.Dot(origin, "1 0 0");
		m_fBarrierOriginY = vector.Dot(origin, "0 1 0");
		m_fBarrierOriginZ = vector.Dot(origin, "0 0 1");

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

			SCR_FactionControlTriggerEntity trigger = SCR_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
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
