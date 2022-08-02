class AISpawnerEntityClass: DAD_SpawnRadiusEntityClass
{
}

class AISpawnerEntity: DAD_SpawnRadiusEntity 
{
	protected ref array<DAD_GroupSpawnPoint> m_SpawnPoints;
	protected static int m_iGroupID = 0;

	override void OnActivate(IEntity ent)
	{
		Resource us   = Resource.Load("{63F01153A435D2BD}Prefabs/MP/Spawning/GroupSpawn_US.et");
		Resource ussr = Resource.Load("{072634CA772ACF5A}Prefabs/MP/Spawning/GroupSpawn_USSR.et");

		super.OnActivate(ent);

		RplComponent rplC = RplComponent.Cast(this.FindComponent(RplComponent));
		if ((rplC && !rplC.IsOwner()) || !Replication.IsServer())
			return;

		foreach (int i, IEntity e : m_Entities)
		{
			SCR_AIGroup group = SCR_AIGroup.Cast(e);
			group.SetName("Squad " + m_iGroupID++);

			string fac = group.m_faction;
			Resource res;
			if (fac == "US")
				res = us;
			else
				res = ussr;

			DAD_GroupSpawnPoint spawn = DAD_GroupSpawnPoint.Cast(SpawnHelpers.SpawnEntity(res, group.GetOrigin()));

			LocalizedString name = group.GetName();
			SCR_UIInfo info = SCR_UIInfo.CreateInfo(name);
			spawn.LinkInfo(info);

			spawn.SetFactionKey(fac);
			spawn.SetGroup(group);
			spawn.LinkInfo(info);

			//----

			AIWaypoint waypoint = AIWaypoint.Cast(GetGame().GetWorld().FindEntityByName("BarrierWaypoint"));
			group.AddWaypoint(waypoint);
		}
	}

};
