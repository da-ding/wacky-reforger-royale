class ScalerGuyClass: DAD_BaseTriggerEntityClass
{
};

class ScalerGuy: DAD_BaseTriggerEntity 
{
	// user script
	AIWaypoint m_eWaypoint;

	float m_fInitialTriggerRadius;
	float m_fInitialBarrierScale;
	
	[RplProp(onRplName: "OnSetSizenator")]
	RplId m_rSizenator;
	
	void OnSetSizenator()
	{
		Print("Oh hey, Sizenator exists!!!! " + m_rSizenator);
		Managed m = Replication.FindItem(m_rSizenator);
		Print("Managed: !!!! " + m);
		RplComponent rplC = RplComponent.Cast(m);
		if (!rplC) return;
		Print("RplComponent:!!!! " + rplC);
		IEntity sizenator = rplC.GetEntity();
		if (!sizenator) return;
		Print("SIZENATOR!!!!! " + sizenator);
		sizenator.SetName("Sizenator");
	}
	
	void ScaleBarrier(float timeSlice)
	{
		Print("Scale Barrier");
		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		//if (rplC.IsProxy()) Print("Proxy is running this code, too");
		//if (Replication.IsClient()) Print("Is Client");
		

		//if (!Replication.IsServer()) return;
		//RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		//if (!rplC || !rplC.IsOwner()) return;

		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");
		IEntity sizenator = w.FindEntityByName("Sizenator");

		if (!barrier || !sizenator)
		{
			Print("No Sizenator!" + m_rSizenator, LogLevel.WARNING);
		}
		

		if (Replication.IsServer())
		{
			DAD_FactionControlTriggerEntity trigger = DAD_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
	
			float delta = (timeSlice * 3);
			float radius = trigger.GetSphereRadius() - delta;

			trigger.SetSphereRadius(radius);
			
			if (m_eWaypoint)
				m_eWaypoint.SetCompletionRadius(radius);
			
			float scale = radius / m_fInitialTriggerRadius;
			
			RplComponent rplBarrier = RplComponent.Cast(sizenator.FindComponent(RplComponent));
			m_rSizenator = rplBarrier.Id();
			Replication.BumpMe();
			
			// Set the barrier size to a fraction of its initial size
			sizenator.SetScale(scale * m_fInitialBarrierScale);
			// slightly raise because greenhouse origin is below ground
			sizenator.SetOrigin(barrier.GetOrigin() + ("0 0.15 0" * delta));
			sizenator.Update();
		}
		barrier.SetScale(sizenator.GetScale());
		barrier.SetOrigin(sizenator.GetOrigin());
		barrier.Update();
		//Print("Updated scane to " + sizenator.GetScale(), LogLevel.NORMAL);
	}
	
	
	void init()
	{
				
		RplComponent rplC = RplComponent.Cast(FindComponent(RplComponent));
		Print("Game Start");
		//if (Replication.IsClient()) Print("Is Client");
		//if (Replication.IsRunning()) Print("Is Running");

		World w = GetGame().GetWorld();
		IEntity barrier = w.FindEntityByName("Barrier");

		if (Replication.IsServer())
		{
						
			DAD_FactionControlTriggerEntity trigger = DAD_FactionControlTriggerEntity.Cast(w.FindEntityByName("VictoryConditionUS"));
			m_fInitialTriggerRadius = trigger.GetSphereRadius();

			m_fInitialBarrierScale = barrier.GetScale();
			
			Resource scoutRes = Resource.Load("{A88F0B6CF25BD1DE}Prefabs/AI/Waypoints/AIWaypoint_Scout.et");
			m_eWaypoint = AIWaypoint.Cast(SpawnHelpers.SpawnEntity(scoutRes, trigger.GetOrigin()));
			m_eWaypoint.SetName("BarrierWaypoint");
			
			Resource rplTest = Resource.Load("{E407D9BCB4B30DC6}Prefabs/MP/Test_RplEntity.et");
			IEntity sizenator = SpawnHelpers.SpawnEntity(rplTest, barrier.GetOrigin());
			sizenator.SetName("Sizenator");
			sizenator.Update();
		}
		else
		{
			IEntity sizenator = w.FindEntityByName("Sizenator");
			if (sizenator) {
			barrier.SetScale(sizenator.GetScale());
			barrier.SetOrigin(sizenator.GetOrigin());
			barrier.Update();
			}
		}
		const float tick = 1 / 16;
		//const float tick = 2;
		Print("End Game Start 1 ");
		GetGame().GetCallqueue().CallLater(ScaleBarrier, tick * 1000, true, tick);
		//GetGame().GetCallqueue().CallLater(m_eBarrier.Update, 500, true);
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
